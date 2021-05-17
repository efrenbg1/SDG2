#ifndef HTTP_H
#define HTTP_H

/*
     libmicrohttpd es una librería escrita por Christian Grothoff.
     Este código está basado en el ejemplo descrito en "fileserver_example.c"
     el cual se encuentra en el repositorio oficial de la librería
*/

#include <microhttpd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <wiringPi.h>
#include "ledDisplay.h"

// Página HTML que se muestra cuando el archivo no se encuentra en la carpeta static
#define no_encontrado "<html><head><title>File not found</title></head><body>File not found</body></html>"

// Puerto TCP en el que escucha el servidor HTTP
#define puerto 8081

// Prototipo funciones (para ser llamadas desde el juego)
void actualizaPantallaHttp(tipo_pantalla *);
int inicializaServidorHttp();

#endif
