#include "teclado_TL04.h"

// DONE cambiar posición teclas al teclado físico que tenemos
char tecladoTL04[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna},
	{-1, NULL, -1, NULL},
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL},
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado)
{
	// DONE inicializar los pines del teclado pongo ejemplo de lo que hay que llamar por cada pin:
	for (int i = 0; i < NUM_COLUMNAS_TECLADO; i++)
	{
		pinMode(p_teclado->columnas[i], OUTPUT);
	}
	for (int i = 0; i < NUM_FILAS_TECLADO; i++)
	{
		pinMode(p_teclado->filas[i], INPUT);
		pullUpDnControl(p_teclado->filas[i], PUD_DOWN);
		wiringPiISR(p_teclado->filas[i], INT_EDGE_RISING, p_teclado->rutinas_ISR[i]);
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna(fsm_t *this)
{
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado *)(this->user_data);

	piLock(KEYBOARD_KEY);
	result = (teclado.flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock(KEYBOARD_KEY);

	return result;
}

int CompruebaTeclaPulsada(fsm_t *this)
{
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado *)(this->user_data);

	piLock(KEYBOARD_KEY);
	result = (teclado.flags & FLAG_TECLA_PULSADA);
	piUnlock(KEYBOARD_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna(fsm_t *this)
{
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado *)(this->user_data);

	piLock(KEYBOARD_KEY);
	teclado.flags &= ~FLAG_TIMEOUT_COLUMNA_TECLADO;

	// DONE apagar la columna actual y excitar la siguiente
	int actual = p_teclado->columna_actual;

	digitalWrite(p_teclado->columnas[actual], LOW);
	actual = actual < 3 ? actual + 1 : 0;
	digitalWrite(p_teclado->columnas[actual], HIGH);

	p_teclado->columna_actual = actual;

	piUnlock(KEYBOARD_KEY);

	tmr_startms((tmr_t *)(teclado.tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);
	// A completar por el alumno
	// ...

	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
}

void ProcesaTeclaPulsada(fsm_t *this)
{
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado *)(this->user_data);

	piLock(KEYBOARD_KEY);
	teclado.flags &= ~FLAG_TECLA_PULSADA;
	char tecla = tecladoTL04[teclado.teclaPulsada.row][teclado.teclaPulsada.col];
	;
	piUnlock(KEYBOARD_KEY);

	piLock(SYSTEM_FLAGS_KEY);
	flags |= FLAG_BOTON;
	if (tecla == '4')
	{
		flags |= FLAG_MOV_IZQUIERDA;
	}
	if (tecla == '6')
	{
		flags |= FLAG_MOV_DERECHA;
	}
	piUnlock(SYSTEM_FLAGS_KEY);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr(void)
{
	piLock(KEYBOARD_KEY);

	if (millis() < teclado.debounceTime[0])
	{
		piUnlock(KEYBOARD_KEY);
		return;
	}

	teclado.teclaPulsada.row = 0;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags |= FLAG_TECLA_PULSADA;

	teclado.debounceTime[0] = millis() + DEBOUNCE_TIME;

	piUnlock(KEYBOARD_KEY);
}

void teclado_fila_2_isr(void)
{
	piLock(KEYBOARD_KEY);

	if (millis() < teclado.debounceTime[1])
	{
		piUnlock(KEYBOARD_KEY);
		return;
	}

	teclado.teclaPulsada.row = 1;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags |= FLAG_TECLA_PULSADA;

	teclado.debounceTime[1] = millis() + DEBOUNCE_TIME;

	piUnlock(KEYBOARD_KEY);
}

void teclado_fila_3_isr(void)
{
	piLock(KEYBOARD_KEY);

	if (millis() < teclado.debounceTime[2])
	{
		piUnlock(KEYBOARD_KEY);
		return;
	}

	teclado.teclaPulsada.row = 2;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags |= FLAG_TECLA_PULSADA;

	teclado.debounceTime[2] = millis() + DEBOUNCE_TIME;

	piUnlock(KEYBOARD_KEY);
}

void teclado_fila_4_isr(void)
{
	piLock(KEYBOARD_KEY);

	if (millis() < teclado.debounceTime[3])
	{
		piUnlock(KEYBOARD_KEY);
		return;
	}

	teclado.teclaPulsada.row = 3;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags |= FLAG_TECLA_PULSADA;

	teclado.debounceTime[3] = millis() + DEBOUNCE_TIME;

	piUnlock(KEYBOARD_KEY);
}

void timer_duracion_columna_isr(union sigval value)
{
	// DONE actualizar flag para que la ISR del teclado cambia de columna
	piLock(KEYBOARD_KEY);
	teclado.flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock(KEYBOARD_KEY);
}
