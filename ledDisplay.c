// DONE hay que poner los pines extra que necesitamos para nuestro display (no usamos el convertor 3 a 8). 16 cables para manejar el display

#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 1, 1, 0},
		{0, 1, 1, 0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 1, 0},
		{0, 0, 1, 1, 1, 1, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
	}};

tipo_pantalla pantalla_final = {
	.matriz = {
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 1, 1, 0},
		{0, 1, 1, 0, 0, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 1, 1, 1, 1, 0, 0},
		{0, 1, 0, 0, 0, 0, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
	}};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay},
	{-1, NULL, -1, NULL},
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay(TipoLedDisplay *led_display)
{
	// DONE inicializar los pines del teclado pongo ejemplo de lo que hay que llamar por cada pin:
	for (int i = 0; i < NUM_COLUMNAS_DISPLAY; i++)
	{
		pinMode(led_display->pines_control_columnas[i], OUTPUT);
		digitalWrite(led_display->pines_control_columnas[i], HIGH);
	}
	for (int i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		pinMode(led_display->filas[i], OUTPUT);
		digitalWrite(led_display->filas[i], LOW);
	}

	led_display->p_columna = 0;
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay(fsm_t *this)
{
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay *)(this->user_data);

	piLock(MATRIX_KEY);
	result = (p_ledDisplay->flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay(fsm_t *this)
{
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay *)(this->user_data);

	// Restablecer flag
	piLock(MATRIX_KEY);
	p_ledDisplay->flags &= ~FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock(MATRIX_KEY);

	piLock(STD_IO_BUFFER_KEY);
	// Apagar columna previa
	int actual = p_ledDisplay->p_columna;
	digitalWrite(p_ledDisplay->pines_control_columnas[actual], HIGH);

	// Actualizar columna
	actual = actual < (NUM_COLUMNAS_DISPLAY - 1) ? actual + 1 : 0;
	p_ledDisplay->p_columna = actual;

	// Filas
	for (int i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		if (p_ledDisplay->pantalla.matriz[i][actual])
		{
			digitalWrite(p_ledDisplay->filas[i], HIGH);
		}
		else
		{
			digitalWrite(p_ledDisplay->filas[i], LOW);
		}
	}

	// Encender nueva columna
	digitalWrite(p_ledDisplay->pines_control_columnas[actual], LOW);
	piUnlock(STD_IO_BUFFER_KEY);

	// Reiniciar TIMER
	tmr_startms((tmr_t *)(p_ledDisplay->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr(union sigval value)
{
	piLock(MATRIX_KEY);
	led_display.flags |= FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock(MATRIX_KEY);
}
