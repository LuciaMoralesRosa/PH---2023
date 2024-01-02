/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				power.c
*/

#include "power.h"

/**
 * Pone el sistema en modo de espera (idle).
 * Esta función coloca el sistema en modo de espera (idle) al establecer el bit
 * 0 del registro de control de energía (PCON). El sistema permanece en espera
 * hasta que se recibe una interrupción.
 */
void power_hal_wait(void){
		// Dormir idle
		PCON |= 0x01;
}

/**
 * Coloca el sistema en modo de suspensión profunda (deep sleep).
 * Esta función configura el temporizador externo de despertar (EXTWAKE) y coloca
 * el sistema en modo de suspensión profunda (deep sleep) al establecer el bit 1
 * del registro de control de energía (PCON). Además, se realiza la transición a
 * la configuración de la Fase de Bloqueo de Fase-Modo de Suspensión Profunda
 * (PLL) mediante la función Switch_to_PLL().
 */
void power_hal_deep_sleep(void) {
		EXTWAKE = 6;
		PCON |= 0x02;
		Switch_to_PLL();
}
