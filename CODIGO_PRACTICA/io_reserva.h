/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				io_reserva.h
*/

#ifndef IO_RESERVA_H
#define IO_RESERVA_H

// Includes
#include <stdint.h>

//Asignacion de GPIO a nombre humano con su longitud
static unsigned int GPIO_OVERFLOW = 31;
static unsigned int GPIO_OVERFLOW_BITS = 1;

static unsigned int GPIO_HELLO_WORD = 0;
static unsigned int GPIO_HELLO_WORD_BITS = 8;

static unsigned int GPIO_VISUALIZAR = 16;
static unsigned int GPIO_VISUALIZAR_BITS = 8;

static unsigned int GPIO_SERIE_ERROR = 30;
static unsigned int GPIO_SERIE_ERROR_BITS = 1;

static unsigned int GPIO_COMANDO_ERROR = 29;
static unsigned int GPIO_COMANDO_ERROR_BITS = 1;


#endif
