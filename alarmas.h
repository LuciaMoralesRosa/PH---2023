/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				alarmas.h
*/

#ifndef ALARMAS_H
#define ALARMAS_H

#include "io_reserva.h"
#include "temporizador_drv.h"
#include "fifo.h"

#define ALARM_MAX 4

struct Alarma{
	enum BOOLEANO activa;
	enum EVENTO_T evento_a_encolar;
	uint32_t auxAlarma;
	uint32_t retardo_ms;
	BOOLEANO periodica;
	uint64_t tiempo_inicio;
};

static struct Alarma alarmas[ALARM_MAX];

uint8_t no_hay_alarmas(void); 
void alarma_inicializar(void);
void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);
void alarma_tratar_eventos(void);

#endif
