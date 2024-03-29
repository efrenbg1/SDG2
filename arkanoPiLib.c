#include "arkanoPiLib.h"

int ladrillos_basico[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = {
	{1, 0, 1, 0, 1, 0, 1, 0},
	{1, 0, 1, 0, 1, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

int ladrillos_2[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 1, 1, 0, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

int ladrillos_3[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};
int ladrillos_4[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};
//------------------------------------------------------
// FUNCIONES DE VISUALIZACION (ACTUALIZACION DEL OBJETO PANTALLA QUE LUEGO USARA EL DISPLAY)
//------------------------------------------------------

void PintaMensajeInicialPantalla(tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial)
{
	// DONE Esta función se tiene que llamar antes de empezar el juego para mostrar el mensaje de bienvenida
	int i, j = 0;

	for (i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
		{
			p_pantalla->matriz[i][j] = p_pantalla_inicial->matriz[i][j];
		}
	}

	return;
}

void PintaPantallaPorTerminal(tipo_pantalla *p_pantalla)
{
	// DONE llamar a esta función en vez de la otra de pintapantalla para depurar
#ifdef __SIN_PSEUDOWIRINGPI__
	int i = 0, j = 0;
	// DONE Quitar o poner? (limpia la pantalla del terminal)
	printf("\e[1;1H\e[2J");
	printf("\n[PANTALLA]\n");
	for (i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
		{
			// DONE Quitar los ceros para que el terminal quede más limpio
			if (p_pantalla->matriz[i][j] == 0)
			{
				printf(" ");
			}
			else
			{
				printf("%d", p_pantalla->matriz[i][j]);
			}
		}
		printf("\n");
	}
	fflush(stdout);
	actualizaPantallaHttp(p_pantalla);
#endif
}

void ReseteaPantalla(tipo_pantalla *p_pantalla)
{
	int i = 0, j = 0;

	for (i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
		{
			p_pantalla->matriz[i][j] = 0;
		}
	}
}

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------

void InicializaLadrillos(tipo_pantalla *p_ladrillos)
{
	int i = 0, j = 0;
	int control_niv = rand() % 4;

	for (i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
		{

			if (control_niv == 0)
			{
				p_ladrillos->matriz[i][j] = ladrillos_basico[i][j];
			}
			else if (control_niv == 1)
			{
				p_ladrillos->matriz[i][j] = ladrillos_2[i][j];
			}
			else if (control_niv == 2)
			{
				p_ladrillos->matriz[i][j] = ladrillos_3[i][j];
			}
			else if (control_niv == 3)
			{
				p_ladrillos->matriz[i][j] = ladrillos_4[i][j];
			}
		}
	}
}

void InicializaPelota(tipo_pelota *p_pelota)
{
	// Aleatorizamos la posicion inicial de la pelota
	p_pelota->x = rand() % NUM_COLUMNAS_DISPLAY;
	//DONE caso que pelota aparezca posicion pala (perdida automatica)
	int inicial = 2; //Generar posicion de la pala entre (NUM_FILAS_DISPLAY-x) e inicial
	int x = 3;
	p_pelota->y = rand() % ((NUM_FILAS_DISPLAY - x) - inicial + 1) + inicial;
	// rand() % ((NUM_FILAS_DISPLAY-2)-inicial+1)-inicial;
	//Pelota inicialmente en el centro de la pantalla
	//p_pelota->x = NUM_COLUMNAS_DISPLAY / 2 - 1;
	//p_pelota->y = NUM_FILAS_DISPLAY / 2 - 1;

	InicializaPosiblesTrayectorias(p_pelota);

	// Trayectoria inicial
	//p_pelota->trayectoria.xv = 0;
	//p_pelota->trayectoria.yv = 1;
	CambiarDireccionPelota(p_pelota, rand() % p_pelota->num_posibles_trayectorias);
}

void InicializaPala(tipo_pala *p_pala)
{
	// Pala inicialmente en el centro de la pantalla
	p_pala->x = NUM_COLUMNAS_DISPLAY / 2 - p_pala->ancho / 2;
	p_pala->y = NUM_FILAS_DISPLAY - 1;
	p_pala->ancho = NUM_COLUMNAS_PALA;
	p_pala->alto = NUM_FILAS_PALA;
}

void InicializaPosiblesTrayectorias(tipo_pelota *p_pelota)
{
	p_pelota->num_posibles_trayectorias = 0;
	p_pelota->posibles_trayectorias[ARRIBA_IZQUIERDA].xv = -1;
	p_pelota->posibles_trayectorias[ARRIBA_IZQUIERDA].yv = 1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ARRIBA].xv = 0;
	p_pelota->posibles_trayectorias[ARRIBA].yv = 1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ARRIBA_DERECHA].xv = 1;
	p_pelota->posibles_trayectorias[ARRIBA_DERECHA].yv = 1;
	p_pelota->num_posibles_trayectorias++;

	p_pelota->posibles_trayectorias[ABAJO_DERECHA].xv = 1;
	p_pelota->posibles_trayectorias[ABAJO_DERECHA].yv = -1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ABAJO].xv = 0;
	p_pelota->posibles_trayectorias[ABAJO].yv = -1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ABAJO_IZQUIERDA].xv = -1;
	p_pelota->posibles_trayectorias[ABAJO_IZQUIERDA].yv = -1;
	p_pelota->num_posibles_trayectorias++;

	//p_pelota->posibles_trayectorias[IZQUIERDA].xv = -1;
	//p_pelota->posibles_trayectorias[IZQUIERDA].yv = 0;
	//p_pelota->num_posibles_trayectorias++;
	//p_pelota->posibles_trayectorias[DERECHA].xv = 1;
	//p_pelota->posibles_trayectorias[DERECHA].yv = 0;
	//p_pelota->num_posibles_trayectorias++;
}

void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla)
{
	int i = 0, j = 0;

	for (i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
		{
			p_pantalla->matriz[i][j] = p_ladrillos->matriz[i][j];
		}
	}
}

void PintaPala(tipo_pala *p_pala, tipo_pantalla *p_pantalla)
{
	int i = 0, j = 0;

	for (i = 0; i < NUM_FILAS_PALA; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_PALA; j++)
		{
			if (((p_pala->y + i >= 0) && (p_pala->y + i < NUM_FILAS_DISPLAY)) &&
				((p_pala->x + j >= 0) && (p_pala->x + j < NUM_COLUMNAS_DISPLAY)))
				p_pantalla->matriz[p_pala->y + i][p_pala->x + j] = 1;
		}
	}
}

void PintaPelota(tipo_pelota *p_pelota, tipo_pantalla *p_pantalla)
{
	if ((p_pelota->x >= 0) && (p_pelota->x < NUM_COLUMNAS_DISPLAY))
	{
		if ((p_pelota->y >= 0) && (p_pelota->y < NUM_FILAS_DISPLAY))
		{
			p_pantalla->matriz[p_pelota->y][p_pelota->x] = 8;
		}
		else
		{
			printf("\n\nPROBLEMAS!!!! posicion y=%d de la pelota INVALIDA!!!\n\n", p_pelota->y);
			fflush(stdout);
		}
	}
	else
	{
		printf("\n\nPROBLEMAS!!!! posicion x=%d de la pelota INVALIDA!!!\n\n", p_pelota->x);
		fflush(stdout);
	}
}

void ActualizaPantalla(tipo_arkanoPi *p_arkanoPi)
{
	// DONE esta función tiene que ser llamada tras mover la pala o la pelota
	// Borro toda la pantalla
	ReseteaPantalla((tipo_pantalla *)(p_arkanoPi->p_pantalla));

	// Pinta los ladrillos
	PintaLadrillos(
		(tipo_pantalla *)(&(p_arkanoPi->ladrillos)),
		(tipo_pantalla *)(p_arkanoPi->p_pantalla));

	// Pinta la pala
	PintaPala(
		(tipo_pala *)(&(p_arkanoPi->pala)),
		(tipo_pantalla *)(p_arkanoPi->p_pantalla));

	// Pinta la pelota
	PintaPelota(
		(tipo_pelota *)(&(p_arkanoPi->pelota)),
		(tipo_pantalla *)(p_arkanoPi->p_pantalla));
}

void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi)
{
	ResetArkanoPi(p_arkanoPi);
	ActualizaPantalla(p_arkanoPi);
}

void ResetArkanoPi(tipo_arkanoPi *p_arkanoPi)
{
	ReseteaPantalla((tipo_pantalla *)(p_arkanoPi->p_pantalla));
	InicializaLadrillos((tipo_pantalla *)(&(p_arkanoPi->ladrillos)));
	InicializaPelota((tipo_pelota *)(&(p_arkanoPi->pelota)));
	InicializaPala((tipo_pala *)(&(p_arkanoPi->pala)));
}

void CambiarDireccionPelota(tipo_pelota *p_pelota, enum t_direccion direccion)
{
	if ((direccion < 0) || (direccion > p_pelota->num_posibles_trayectorias))
	{
		printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
		return;
	}
	else
	{
		p_pelota->trayectoria.xv = p_pelota->posibles_trayectorias[direccion].xv;
		p_pelota->trayectoria.yv = p_pelota->posibles_trayectorias[direccion].yv;
	}
}

void ActualizaPosicionPala(tipo_pala *p_pala, enum t_direccion direccion, tipo_pantalla *p_pantalla)
{
	switch (direccion)
	{
	case DERECHA:
		// Dejamos que la pala rebase parcialmente el límite del area de juego
		if (p_pala->x + 1 + p_pala->ancho <= NUM_COLUMNAS_DISPLAY + 2)
			p_pala->x = p_pala->x + 1;
		break;
	case IZQUIERDA:
		// Dejamos que la pala rebase parcialmente el límite del area de juego
		if (p_pala->x - 1 >= -2)
			p_pala->x = p_pala->x - 1;

		// else if (p_pala->x - 1 >= -1)
		// {
		// 	p_pala->x = p_pala->x - 1;
		// 	p_pantalla->matriz[4][4] = 1;
		// }
		break;
	default:
		printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
		break;
	}
}

void ActualizaPosicionPelota(tipo_pelota *p_pelota)
{
	p_pelota->x += p_pelota->trayectoria.xv;
	p_pelota->y += p_pelota->trayectoria.yv;
}

int CompruebaReboteLadrillo(tipo_arkanoPi *p_arkanoPi)
{
	int p_posible_ladrillo_x = 0;
	int p_posible_ladrillo_y = 0;

	p_posible_ladrillo_x = p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv;

	if ((p_posible_ladrillo_x < 0) || (p_posible_ladrillo_x >= NUM_COLUMNAS_DISPLAY))
	{
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_x = %d!!!\n\n", p_posible_ladrillo_x);
		fflush(stdout);
		exit(-1);
	}

	p_posible_ladrillo_y = p_arkanoPi->pelota.y + p_arkanoPi->pelota.trayectoria.yv;

	if ((p_posible_ladrillo_y < 0) || (p_posible_ladrillo_y >= NUM_FILAS_DISPLAY))
	{
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_y = %d!!!\n\n", p_posible_ladrillo_y);
		fflush(stdout);
	}

	if (p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] > 0)
	{
		// La pelota ha entrado en el area de ladrillos
		// y descontamos el numero de golpes que resta para destruir el ladrillo
		p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] = p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] - 1;

		return 1;
	}
	return 0;
}

int CompruebaReboteParedesVerticales(tipo_arkanoPi arkanoPi)
{
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if ((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= NUM_COLUMNAS_DISPLAY) ||
		(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < 0))
	{
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaReboteTecho(tipo_arkanoPi arkanoPi)
{
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if (arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < 0)
	{
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaRebotePala(tipo_arkanoPi arkanoPi)
{
	if (arkanoPi.pelota.trayectoria.yv > 0)
	{ // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= arkanoPi.pala.x) &&
			(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < arkanoPi.pala.x + NUM_COLUMNAS_PALA))
		{
			if ((arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= arkanoPi.pala.y) &&
				(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < arkanoPi.pala.y + NUM_FILAS_PALA))
			{
				return 1;
			}
		}
	}
	return 0;
}

int CompruebaFallo(tipo_arkanoPi arkanoPi)
{
	// Comprobamos si no hemos conseguido devolver la pelota
	if (arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= NUM_FILAS_DISPLAY)
	{
		// Hemos fallado
		return 1;
	}
	return 0;
}

int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos)
{
	int i = 0, j = 0;
	int numLadrillosRestantes;

	numLadrillosRestantes = 0;

	for (i = 0; i < NUM_FILAS_DISPLAY; i++)
	{
		for (j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
		{
			if (p_ladrillos->matriz[i][j] != 0)
			{
				numLadrillosRestantes++;
			}
		}
	}

	return numLadrillosRestantes;
}

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaBotonPulsado(fsm_t *this)
{
	int result = 0;
	// DONE A completar por el alumno
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_BOTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);
	if (animar(p_arkanoPi->p_pantalla))
	{
		piLock(STD_IO_BUFFER_KEY);
		PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
		printf("\nPusle cualquier tecla para empezar...\n");
		fflush(stdout);
		piUnlock(STD_IO_BUFFER_KEY);
	}

	return result;
}

int CompruebaMovimientoIzquierda(fsm_t *this)
{
	int result = 0;
	// DONE A completar por el alumno
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_IZQUIERDA);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaMovimientoDerecha(fsm_t *this)
{
	int result = 0;
	// DONE A completar por el alumno
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_DERECHA);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTimeoutActualizacionJuego(fsm_t *this)
{
	int result = 0;

	// DONE A completar por el alumno
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMER_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego(fsm_t *this)
{
	int result = 0;

	// DONE A completar por el alumno
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_FIN_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicialización de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.

void InicializaJuego(fsm_t *this)
{
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags = 0;
	piUnlock(SYSTEM_FLAGS_KEY);

	// DONE A completar por el alumno
	piLock(STD_IO_BUFFER_KEY);
	InicializaArkanoPi(p_arkanoPi);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY);

	tmr_startms((tmr_t *)(p_arkanoPi->tmr_actualizacion_juego), TIMEOUT_ACTUALIZA_JUEGO);
}

// void MuevePalaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la pala.
// Debe garantizar la viabilidad del mismo mediante la comprobación
// de que la nueva posición correspondiente a la pala no suponga
// que ésta rebase o exceda los límites definidos para el área de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida).

void MuevePalaIzquierda(fsm_t *this)
{
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);

	// DONE Comprobar que la pala se pueda mover a la izquierda (uno de los leds tiene que estar visible)
	piLock(SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_MOV_IZQUIERDA;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	ActualizaPosicionPala(&p_arkanoPi->pala, IZQUIERDA, (tipo_pantalla *)(p_arkanoPi->p_pantalla));
	ActualizaPantalla(p_arkanoPi);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY);
}

// void MuevePalaDerecha (void): función similar a la anterior
// encargada del movimiento hacia la derecha.

void MuevePalaDerecha(fsm_t *this)
{
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);

	// DONE Comprobar que la pala se pueda mover a la derecha (uno de los leds tiene que estar visible)
	piLock(SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_MOV_DERECHA;
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(STD_IO_BUFFER_KEY);
	ActualizaPosicionPala(&p_arkanoPi->pala, DERECHA, (tipo_pantalla *)(p_arkanoPi->p_pantalla));
	ActualizaPantalla(p_arkanoPi);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY);
}

// void ActualizarJuego (void): función encargada de actualizar la
// posición de la pelota conforme a la trayectoria definida para ésta.
// Para ello deberá identificar los posibles rebotes de la pelota para,
// en ese caso, modificar su correspondiente trayectoria (los rebotes
// detectados contra alguno de los ladrillos implicarán adicionalmente
// la eliminación del ladrillo). Del mismo modo, deberá también
// identificar las situaciones en las que se dé por finalizada la partida:
// bien porque el jugador no consiga devolver la pelota, y por tanto ésta
// rebase el límite inferior del área de juego, bien porque se agoten
// los ladrillos visibles en el área de juego.

void ActualizarJuego(fsm_t *this)
{
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);

	// DONE Hacer lo que dice arriba ^
	piLock(SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_TIMER_JUEGO;
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPelota(p_arkanoPi);
	int control_dif = CalculaLadrillosRestantes((tipo_pantalla *)(&(p_arkanoPi->ladrillos)));
	if (control_dif > 8)
	{
		tmr_startms((tmr_t *)(p_arkanoPi->tmr_actualizacion_juego), TIMEOUT_ACTUALIZA_JUEGO);
	}
	else if (control_dif > 4)
	{
		tmr_startms((tmr_t *)(p_arkanoPi->tmr_actualizacion_juego), TIMEOUT_ACTUALIZA_JUEGO_2);
	}
	else
	{
		tmr_startms((tmr_t *)(p_arkanoPi->tmr_actualizacion_juego), TIMEOUT_ACTUALIZA_JUEGO_3);
	}
}

// void FinalJuego (void): función encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.

void FinalJuego(fsm_t *this)
{
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);

	tmr_stop(p_arkanoPi->tmr_actualizacion_juego);
	p_arkanoPi->tmr_actualizacion_juego = tmr_new(tmr_actualizacion_juego_isr);

	piLock(STD_IO_BUFFER_KEY);
	if (CompruebaDerrota(p_arkanoPi))
	{
		PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &pantalla_final);
		//printf("\nHas perdido\n");
		//fflush(stdout);
	}
	else
	{
		PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &pantalla_inicial);
		//printf("\nEres un champion\n");
		//fflush(stdout);
	}

	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY);

	piLock(SYSTEM_FLAGS_KEY);
	flags = 0;
	piUnlock(SYSTEM_FLAGS_KEY);

	animarFinal();
}

//void ReseteaJuego (void): función encargada de llevar a cabo la
// reinicialización de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.

void ReseteaJuego(fsm_t *this)
{
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi *)(this->user_data);

	// DONE resetear los contadores del juego y la pantalla
	InicializaArkanoPi(p_arkanoPi);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void tmr_actualizacion_juego_isr(union sigval value)
{
	piLock(SYSTEM_FLAGS_KEY);
	flags |= FLAG_TIMER_JUEGO;
	piUnlock(SYSTEM_FLAGS_KEY);
}
