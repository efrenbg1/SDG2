#include "arkanoPiLib.h"
#include "arkanoPi.h"

void timer_isr(union sigval value)
{
    piLock(SYSTEM_FLAGS_KEY);
    flags |= FLAG_TIMER_JUEGO;
    piUnlock(SYSTEM_FLAGS_KEY);
}

void EncenderTimer(tipo_arkanoPi *p_arkanoPi)
{
    p_arkanoPi->tmr_actualizacion_juego = tmr_new(timer_isr);
    tmr_startms(p_arkanoPi->tmr_actualizacion_juego, 1000);
}

void ApagarTimer(tipo_arkanoPi *p_arkanoPi)
{
    tmr_destroy(p_arkanoPi->tmr_actualizacion_juego);
}
