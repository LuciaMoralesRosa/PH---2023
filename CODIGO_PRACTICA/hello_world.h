/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				hello_world.h
*/

#ifndef HELLO_WORLD_H
#define HELLO_WORLD_H

// Modulos empleados
#include "gpio_hal.h"
#include "alarma.h"
#include "WD.h"

/**
 * Inicializa la funcionalidad "Hello World".
 * Esta función configura la funcionalidad "Hello World", activando una alarma
 * con un retardo específico y estableciendo la función de retorno de llamada.
 *
 * @param funcion_callback Función de retorno de llamada para la funcionalidad "Hello World".
 */
void hello_world_inicializar(void (*funcion_callback)());

/**
 * Trata el evento "Latido" y encola el evento "Visualizar Hello".
 * Esta función se encarga de tratar el evento "Latido" (ev_LATIDO) mediante el
 * alimentado del watchdog y luego encola el evento "Visualizar Hello"
 * (ev_VISUALIZAR_HELLO). La función de retorno de llamada correspondiente a
 * "Hello World" es invocada para realizar las acciones necesarias al tratar el evento.
 */
void hello_world_tratar_evento(void);

#endif
