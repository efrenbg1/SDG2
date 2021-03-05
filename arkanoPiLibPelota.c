#include "arkanoPiLib.h"

void ActualizaPelota(tipo_arkanoPi *p_arkanoPi)
{

    // DONE Caso perder
    // DONE Caso ganar
    if (CompruebaDerrota(p_arkanoPi) || CompruebaVictoria(p_arkanoPi))
    {
        piLock(SYSTEM_FLAGS_KEY);
        flags |= FLAG_FIN_JUEGO;
        piUnlock(SYSTEM_FLAGS_KEY);
        return;
    }

    // DONE rebota mal con esquina de tres ladrillos
    // 111
    // 181
    // 000

    // TODO Cambiar direcciones por las que nos dan

    if (CompruebaReboteParedesVerticales(*p_arkanoPi))
    {
        p_arkanoPi->pelota.trayectoria.xv *= -1;
    }

    if (CompruebaReboteTecho(*p_arkanoPi))
    {
        p_arkanoPi->pelota.trayectoria.yv *= -1;
    }
    else if (CompruebaReboteLadrillo(p_arkanoPi))
    {
        p_arkanoPi->pelota.trayectoria.yv *= -1;
    }
    else if (CompruebaRebotePala(*p_arkanoPi))
    {
        int pelota = p_arkanoPi->pelota.x;
        int pala = p_arkanoPi->pala.x + 1;
        if (pelota < pala - 1)
        {
            p_arkanoPi->pelota.trayectoria.yv *= -1;
            p_arkanoPi->pelota.trayectoria.xv *= -1;
        }
        else if (pelota == pala - 1)
        {
            if (p_arkanoPi->pelota.trayectoria.xv == 0)
            {
                p_arkanoPi->pelota.trayectoria.yv *= -1;
                p_arkanoPi->pelota.trayectoria.xv = -1;
            }
            else
            {
                p_arkanoPi->pelota.trayectoria.yv *= -1;
                p_arkanoPi->pelota.trayectoria.xv = 0;
            }
        }
        else if (pelota == pala)
        {
            p_arkanoPi->pelota.trayectoria.yv *= -1;
        }
        else if (pelota == pala + 1)
        {
            if (p_arkanoPi->pelota.trayectoria.xv == 0)
            {
                p_arkanoPi->pelota.trayectoria.yv *= -1;
                p_arkanoPi->pelota.trayectoria.xv = 1;
            }
            else
            {
                p_arkanoPi->pelota.trayectoria.yv *= -1;
                p_arkanoPi->pelota.trayectoria.xv = 0;
            }
        }
        else if (pelota > pala + 1)
        {
            p_arkanoPi->pelota.trayectoria.yv *= -1;
            p_arkanoPi->pelota.trayectoria.xv *= -1;
        }
    }

    if (CompruebaReboteParedesVerticales(*p_arkanoPi))
    {
        p_arkanoPi->pelota.trayectoria.xv *= -1;
    }

    piLock(STD_IO_BUFFER_KEY);
    ActualizaPosicionPelota(&p_arkanoPi->pelota);
    ActualizaPantalla(p_arkanoPi);
    PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
    piUnlock(STD_IO_BUFFER_KEY);
}

int CompruebaDerrota(tipo_arkanoPi *p_arkanoPi)
{
    return p_arkanoPi->pelota.y + p_arkanoPi->pelota.trayectoria.yv > 6;
}

int CompruebaVictoria(tipo_arkanoPi *p_arkanoPi)
{
    for (int i = 0; i < NUM_COLUMNAS_DISPLAY; i++)
    {
        for (int j = 0; j < NUM_FILAS_DISPLAY; j++)
        {
            if (p_arkanoPi->ladrillos.matriz[j][i] > 0)
            {
                return 0;
            }
        }
    }
    return 1;
}