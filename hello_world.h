/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				hello_world.h
*/

#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

#include "io_reserva.h"
#include "gpio_hal.h"

void hello_world_inicializar(int GPIO_HELLO_WORD, int GPIO_HELLO_WORD_BITS);
void hello_world_tick_tack(void);

#endif
