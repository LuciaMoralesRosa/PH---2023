/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				fifo.h
*/

#ifndef FIFO_H
#define FIFO_H

#define SIZE 32

#include <stdint.h>
#include "gpio_hal.h"
#include "io_reserva.h"

/**Conjunto de eventos posibles. En el fichero de cabecera se incluirá el tipo
de datos y el conjunto de posibles eventos identificados con nombre humano.
Reservemos el ID VOID con valor cero para inicializar la cola				**/

enum EVENTO_T {
	ID_VOID = 0,
	Timer0 = 1,
	Timer1 = 2,
};

struct Evento {
	enum EVENTO_T Evento_ID;
	uint32_t auxData;
};

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);
void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData);
uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t* auxData);
uint32_t FIFO_estadisticas(enum EVENTO_T ID_evento);


#endif
