/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				watchdog.h
*/

#ifndef WATCHDOG_H
#define WATCHDOG_H

// Includes
#include <LPC210x.H>

// Modulos empleados
#include "swi.h"

/**
 * Inicializa el Watchdog Timer para que resetee el procesador dentro de sec
 * segundos si no se le "alimenta".
 * Esta función configura el Watchdog Timer para que reinicie el procesador si
 * no se le "alimenta" dentro del tiempo especificado. Se asigna el tiempo de
 * cuenta con el registro WDTC, y se activan los bits de reset y enable en el
 * registro WDMOD.
 *
 * @param sec El tiempo en segundos antes de que el Watchdog Timer reinicie el procesador.
 */
void WD_hal_inicializar(int sec);

/**
 * Alimenta al Watchdog Timer.
 * Esta función alimenta el Watchdog Timer para evitar que alcance su tiempo de
 * espera y reinicie el procesador. Se desactivan temporalmente las
 * interrupciones durante las llamadas consecutivas para garantizar la
 * secuencia correcta.
 */
void WD_hal_feed(void);

/**
 * Test con un bucle infinito para comprobar que el Watchdog funciona.
 */
void WD_hal_test(void);

#endif

