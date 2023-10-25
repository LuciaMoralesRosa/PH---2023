/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				timer0_drv.h
 */

#ifndef TIMER0_DRV_H
#define TIMER0_DRV_H

#include <LPC210x.H>
#include "temporizador_hal.h"
#include <stdint.h>

// Programa un contador para que pueda ser utilizado.
void timer0_drv_iniciar(void);

// Inicia la cuenta de un contador de forma indefinida.
void timer0_drv_empezar(void);

// Lee el tiempo que lleva contando el contador desde que se ejecutó
// temporizador_hal_empezar() y lo devuelve en ticks
uint64_t timer0_drv_leer(void);

// Detiene el contador y devuelve el tiempo en ticks transcurrido desde
// el último temporizador_hal_empezar()
uint64_t timer0_drv_parar(void);

void timer1_drv_reloj(uint32_t periodo, void (*funcion_callback)());

#endif
