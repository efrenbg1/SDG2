#include "arkanoPiLib.h"

void ActualizaPelota(tipo_arkanoPi *p_arkanoPi)
{

    // DONE Caso perder
    if (p_arkanoPi->pelota.y + p_arkanoPi->pelota.trayectoria.yv > 6)
    {
        piLock(SYSTEM_FLAGS_KEY);
        flags |= FLAG_FIN_JUEGO;
        piUnlock(SYSTEM_FLAGS_KEY);
        return;
    }
    // TODO Caso ganar

    // DONE rebota mal con esquina de tres ladrillos
    // 111
    // 181
    // 000

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
        //ActualizaPosicionPelota(&p_arkanoPi->pelota);
        p_arkanoPi->pelota.trayectoria.yv *= -1;
        // TODO if(){

        // }
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
