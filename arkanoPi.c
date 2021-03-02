
#include "arkanoPi.h"

int flags = 0;

TipoSistema sistema;

// Declaracion del objeto teclado
TipoTeclado teclado = {
	.columnas = {
		// A completar por el alumno...
		// ...
	},
	.filas = {
		// A completar por el alumno...
		// ...
	},
	.rutinas_ISR = {
		// A completar por el alumno...
		// ...
	},

	// A completar por el alumno...
	// ...
};

// Declaracion del objeto display
TipoLedDisplay led_display = {
	.pines_control_columnas = {
		// A completar por el alumno...
		// ...
	},
	.filas = {
		// A completar por el alumno...
		// ...
	},
	// A completar por el alumno...
	// ...
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
	p_sistema->arkanoPi.p_pantalla = &(led_display.pantalla);
	// Lanzamos thread para exploracion del teclado convencional del PC
	result = piThreadCreate(thread_explora_teclado_PC);
	if (result != 0)
	{
		printf("Thread didn't start!!!\n");
		return -1;
	}

	return result;
}

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------

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

void timer_isr(union sigval value)
{
	flags |= FLAG_TIMER_JUEGO;
}

int main()
{
	unsigned int next;

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

	// Configuracion e incializacion del sistema
	ConfiguraInicializaSistema(&sistema);

	tmr_t *tmr = tmr_new(timer_isr);
	tmr_startms(tmr, 1000);

	fsm_t *arkanoPi_fsm = fsm_new(WAIT_START, arkanoPi, &sistema);

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

		// A completar por el alumno...
		// ...

		next += CLK_MS;
		delay_until(next);
	}

	fsm_destroy(arkanoPi_fsm);
}
