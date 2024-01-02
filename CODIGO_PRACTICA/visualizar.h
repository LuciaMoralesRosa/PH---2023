/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				visualizar.h
 */

#ifndef VISUALIZAR_H
#define VISUALIZAR_H

// Modulos empleados
#include "io_reserva.h"
#include "gpio_hal.h"

/**
 * Inicializa la visualización.
 * Esta función inicializa la visualización configurando el sentido del pin de
 * salida y estableciendo su valor inicial en 0.
 */
void visualizar_inicializar(void);

/**
 * Visualiza un contador en la pantalla.
 *
 * @param cuenta El valor a mostrar en la pantalla.
 */
void visualizar_cuenta(uint32_t cuenta);

/**
 * Incrementa el valor visualizado en la pantalla de saludo.
 * Lee el valor actual del pin de la pantalla de saludo, incrementa su valor y
 * lo escribe de nuevo en el pin.
 */
void visualizar_hello(void);

#endif
