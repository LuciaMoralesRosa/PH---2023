/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				temporizador_hal.h
 */

#ifndef TEMPORIZADOR_HAL_H
#define TEMPORIZADOR_HAL_H

// Includes
#include <LPC210x.H>
#include <stdint.h>

// Definicion de la constante temporizador_hal_ticks2us
static unsigned int temporizador_hal_ticks2us = 15000;

/**
 * Programa un contador para ser utilizado.
 */
void temporizador_hal_iniciar(void);

/**
 * Inicia el temporizador y habilita las interrupciones asociadas.
 */
void temporizador_hal_empezar(void);

/**
 * Lee el tiempo que lleva contando el contador desde que se ejecutó
 * temporizador_hal_empezar() y lo devuelve en microsegundos.
 * @return Tiempo transcurrido en microsegundos.
 */
uint64_t temporizador_hal_leer(void);

/**
 * Detiene el temporizador y devuelve el tiempo transcurrido desde el último
 * temporizador_hal_empezar().
 * @return Tiempo transcurrido en microsegundos.
 */
uint64_t temporizador_hal_parar(void);

/**
 * Programa el reloj para que llame a la función de callback cada periodo.
 * Si el periodo es cero, se para el temporizador.
 */
void temporizador_hal_reloj(uint32_t periodo, void (*funcion_callback)());

#endif
