/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				io_reserva.h
*/

#ifndef IO_RESERVA_H
#define IO_RESERVA_H

#include <stdint.h>

typedef int BOOLEANO;

enum BOOLEANO{
	false = 0,
	true = 1,
};

enum EVENTO_T {
	ID_VOID = 0,
	Timer0 = 1,
	Timer1 = 2,
	ALARMA_OVERFLOW = 3,
};

struct Evento {
	enum EVENTO_T Evento_ID;
	uint32_t auxData;
};



static unsigned int GPIO_OVERFLOW = 31;
static unsigned int GPIO_OVERFLOW_BITS = 1;

static unsigned int GPIO_HELLO_WORD = 0;
static unsigned int GPIO_HELLO_WORD_BITS = 8;

#endif
