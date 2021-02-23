#include "animar.h"

int timeout = 1000;

// Cursores de la animación
int fila = 0, columna = 0, direccion = 1, direccion2 = 1;
int pantalla[7][8] = {0};

void animarFinal()
{
    timeout = 10000;
}

int animar(tipo_pantalla *p_pantalla)
{
    if (timeout > 0)
    {
        timeout--;
        return 0;
    }
    timeout = 100;

    pantalla[fila][columna] = direccion ? 1 : 0;

    columna += direccion2 ? 1 : -1;

    if (columna > 7 || columna < 0)
    {
        fila += direccion ? 1 : -1;
        direccion2 = !direccion2;
        columna += direccion2 ? 1 : -1;
    }

    if (fila > 6)
    {
        direccion = 0;
        fila = 6;
        columna = 7;
    }
    if (fila < 0)
    {
        direccion = 1;
        fila = 0;
        columna = 0;
    }

    // Copiar matriz a pantalla
    for (int i = 0; i < NUM_FILAS_DISPLAY; i++)
    {
        for (int j = 0; j < NUM_COLUMNAS_DISPLAY; j++)
        {
            p_pantalla->matriz[i][j] = pantalla[i][j];
        }
    }

    return 1;
}