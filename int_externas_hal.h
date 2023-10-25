/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				int_externas_hal.h
 */

#ifndef INT_EXTERNAS_HAL_H
#define INT_EXTERNAS_HAL_H

#include <LPC210x.H>
#include <stdint.h>
#include "botones.h"

// Inicializa eint1
void eint1_init (void);

// Elimina la pulsacion
void eint1_clear_nueva_pulsacion(void);

// Inicializa eint2
void eint2_init (void);

// Elimina la pulsacion
void eint2_clear_nueva_pulsacion(void);

// Reactiva las interrupciones
void eint1_interrupcion_manejada(void);
void eint2_interrupcion_manejada(void);

uint8_t devolver(void);

#endif
