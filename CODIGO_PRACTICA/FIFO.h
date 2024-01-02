/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				fifo.h
*/

#ifndef FIFO_H
#define FIFO_H

// Constante que define el tamaño de la cola
#define SIZE 32

// Includes
#include <stdint.h>

// Modulos empleados
#include "gpio_hal.h"
#include "eventos.h"
#include "swi.h"

// Definicion de la estructura Evento
struct Evento {
	enum EVENTO_T Evento_ID;
	uint32_t auxData;
};

/**
 * Inicializa la cola FIFO y las estadísticas de eventos.
 * Esta función inicializa la cola FIFO, el LED de overflow y las estadísticas
 * de eventos.
 *
 * @param pin_overflow Pin GPIO asociado al LED de overflow.
 */
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow);

/**
 * Encola un evento en la cola FIFO.
 * Esta función añade un nuevo evento a la cola FIFO, actualiza los índices de
 * la cola y las estadísticas.
 *
 * @param ID_evento Identificador del evento a encolar.
 * @param auxData Datos auxiliares asociados al evento.
 */
void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData);

/**
 * Extrae el evento más antiguo sin procesar de la cola FIFO.
 * Esta función devuelve un valor distinto de cero y el evento más antiguo sin
 * procesar por referencia si hay eventos en la cola. Cero indica que la cola
 * está vacía y no se ha devuelto ningún evento.
 *
 * @param ID_evento Puntero para almacenar el identificador del evento extraído.
 * @param auxData Puntero para almacenar los datos auxiliares del evento extraído.
 * @return Valor distinto de cero si hay eventos sin procesar, cero si la cola está vacía.
 */
uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t* auxData);

/**
 * Obtiene las estadísticas de eventos de la cola FIFO.
 * Esta función devuelve las estadísticas de eventos asociadas a un evento
 * específico o al total de eventos, según el identificador de evento proporcionado.
 *
 * @param ID_evento Identificador del evento del cual se desean obtener las estadísticas.
 * @return Valor de las estadísticas asociadas al evento específico o al total de eventos.
 */
uint32_t FIFO_estadisticas(enum EVENTO_T ID_evento);

#endif
