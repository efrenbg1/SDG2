
#include "arkanoPi.h"

int flags = 0;

TipoSistema sistema;

// Declaracion del objeto teclado
TipoTeclado teclado = {
	.columnas = {GPIO_KEYBOARD_COL_1, GPIO_KEYBOARD_COL_2, GPIO_KEYBOARD_COL_3, GPIO_KEYBOARD_COL_4},
	.filas = {GPIO_KEYBOARD_ROW_1, GPIO_KEYBOARD_ROW_2, GPIO_KEYBOARD_ROW_3, GPIO_KEYBOARD_ROW_4},
	.rutinas_ISR = {teclado_fila_1_isr, teclado_fila_2_isr, teclado_fila_3_isr, teclado_fila_4_isr},

	// TODO Falta algo aquí? (A completar por el alumno...)
};

// Declaracion del objeto display
TipoLedDisplay led_display = {
	.pines_control_columnas = {
		GPIO_LED_DISPLAY_COL_1, GPIO_LED_DISPLAY_COL_2, GPIO_LED_DISPLAY_COL_3, GPIO_LED_DISPLAY_COL_4, GPIO_LED_DISPLAY_COL_5, GPIO_LED_DISPLAY_COL_6, GPIO_LED_DISPLAY_COL_7, GPIO_LED_DISPLAY_COL_8},
	.filas = {GPIO_LED_DISPLAY_ROW_1, GPIO_LED_DISPLAY_ROW_2, GPIO_LED_DISPLAY_ROW_3, GPIO_LED_DISPLAY_ROW_4, GPIO_LED_DISPLAY_ROW_5, GPIO_LED_DISPLAY_ROW_6, GPIO_LED_DISPLAY_ROW_7},

	// TODO Falta algo aquí? (A completar por el alumno...)
};

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion
// e inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
// como el thread de exploración del teclado del PC
int ConfiguraInicializaSistema(TipoSistema *p_sistema)
{
	int result = 0;
	// DONE inizializar HW (pantalla y teclado?)
	wiringPiSetupGpio();
	InicializaTeclado(&teclado);
	InicializaLedDisplay(&led_display);
	p_sistema->arkanoPi.p_pantalla = &(led_display.pantalla);

	// Lanzamos thread para exploracion del teclado convencional del PC
	/*result = piThreadCreate(thread_explora_teclado_PC);
	if (result != 0)
	{
		printf("Thread didn't start!!!\n");
		return -1;
	}*/

	// DONE Inicializar timer asociado al teclado numérico
	teclado.tmr_duracion_columna = tmr_new(timer_duracion_columna_isr);
	tmr_startms((tmr_t *)(teclado.tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

	/*led_display.tmr_refresco_display = tmr_new(timer_refresco_display_isr);
	tmr_startms((tmr_t *)(led_display.tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);*/

	result = piThreadCreate(thread_pantalla);
	if (result != 0)
	{
		printf("Thread didn't start!!!\n");
		return -1;
	}

	// DONE Inicializar los gpio
	// TODO hay que poner los pines extra que necesitamos para nuestro display (no usamos el convertor 3 a 8). Esto no va aquí pero en algún sitio lo tenía que poner

	return result;
}

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------

PI_THREAD(thread_pantalla)
{
	fsm_t *display_fsm = fsm_new(DISPLAY_ESPERA_COLUMNA, fsm_trans_excitacion_display, &led_display);

	for (;;)
	{
		fsm_fire(display_fsm);
		delay(1);
	}

	fsm_destroy(display_fsm);
}

PI_THREAD(thread_explora_teclado_PC)
{
	int teclaPulsada;

	while (1)
	{
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock(STD_IO_BUFFER_KEY);

		if (kbhit())
		{
			teclaPulsada = kbread();

			// DONE A completar por el alumno...
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_BOTON;
			switch (teclaPulsada)
			{
			case 'a':
				flags |= FLAG_MOV_IZQUIERDA;
				break;
			case 'c':
				flags |= FLAG_TIMER_JUEGO;
				break;
			case 'd':
				flags |= FLAG_MOV_DERECHA;
				break;
			case 'q':
				flags |= FLAG_FIN_JUEGO;
				fflush(stdout);
				exit(0);
				break;
			default:
				printf("INVALID KEY!!!\n");
				break;
			}
			piUnlock(SYSTEM_FLAGS_KEY);
		}
		piUnlock(STD_IO_BUFFER_KEY);
	}
}

// wait until next_activation (absolute time)
void delay_until(unsigned int next)
{
	unsigned int now = millis();
	if (next > now)
	{
		delay(next - now);
	}
}

int main()
{
	unsigned int next;

	// Configuracion e incializacion del sistema
	ConfiguraInicializaSistema(&sistema);

	// Maquina de estados: lista de transiciones
	// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
	fsm_trans_t arkanoPi[] = {
		{WAIT_START, CompruebaBotonPulsado, WAIT_PUSH, InicializaJuego},
		{WAIT_PUSH, CompruebaTimeoutActualizacionJuego, WAIT_PUSH, ActualizarJuego},
		{WAIT_PUSH, CompruebaMovimientoIzquierda, WAIT_PUSH, MuevePalaIzquierda},
		{WAIT_PUSH, CompruebaMovimientoDerecha, WAIT_PUSH, MuevePalaDerecha},
		{WAIT_PUSH, CompruebaFinalJuego, WAIT_END, FinalJuego},
		{WAIT_END, CompruebaBotonPulsado, WAIT_START, ReseteaJuego},
		{-1, NULL, -1, NULL},
	};
	/*fsm_trans_t tabla_tecla[] = {
		{TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada,
		 TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
		{-1, NULL, -1, NULL}};*/
	fsm_t *arkanoPi_fsm = fsm_new(WAIT_START, arkanoPi, &sistema);
	fsm_t *teclado_fsm = fsm_new(TECLADO_ESPERA_COLUMNA, fsm_trans_excitacion_columnas, &teclado);
	fsm_t *tecla_fsm = fsm_new(TECLADO_ESPERA_TECLA, fsm_trans_deteccion_pulsaciones, &teclado);

	// A completar por el alumno...
	// DONE Poner mensaje de bienvenida
	piLock(STD_IO_BUFFER_KEY);
	PintaMensajeInicialPantalla(sistema.arkanoPi.p_pantalla, &pantalla_inicial);
	PintaPantallaPorTerminal(sistema.arkanoPi.p_pantalla);
	printf("\nPusle cualquier tecla para empezar...\n");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);

	next = millis();
	while (1)
	{
		fsm_fire(arkanoPi_fsm);
		fsm_fire(teclado_fsm);
		fsm_fire(tecla_fsm);

		next += CLK_MS;
		delay_until(next);
	}

	fsm_destroy(arkanoPi_fsm);
	fsm_destroy(teclado_fsm);
	fsm_destroy(tecla_fsm);
}
