/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				alarma.h
*/

#ifndef ALARMAS_H
#define ALARMAS_H

//Modulos empleados
#include "temporizador_drv.h"
#include "definiciones.h"
#include "eventos.h"
#include "swi.h"

//Constante que define el numero maximo de alarmas a emplear
#define ALARM_MAX 5

//Definicion de la estructura Alarma
struct Alarma{
		int activa;
		enum EVENTO_T evento_a_encolar;
		uint32_t auxAlarma;
		uint32_t retardo_ms;
		int periodica;
		uint64_t tiempo_inicio;
};

// Arreglo de estructuras Alarma para almacenar las alarmas
static struct Alarma alarmas[ALARM_MAX];

/**
 * Verifica si no hay alarmas activas.
 * Esta función recorre el array de alarmas y verifica si hay alguna activa.
 *
 * @return Devuelve TRUE si no hay alarmas activas, FALSE en caso contrario.
 */
uint8_t no_hay_alarmas(void);

/**
 * Inicializa el sistema de alarmas.
 * Esta función configura el sistema de alarmas, establece la función de
 * callback, inicia el temporizador y prepara la cola de alarmas.
 *
 * @param funcion_callback Puntero a la función de callback que se ejecutará al
 * 						   activarse una alarma.
 */
void alarma_inicializar(void (*funcion_callback)());

/**
 * Activa o reprograma una alarma en el sistema.
 * Esta función activa o reprograma una alarma en función del evento
 * proporcionado, el retardo y los datos auxiliares. También puede cancelar una
 * alarma si el retardo es cero.
 *
 * @param ID_evento Identificador del evento a insertar en la cola al vencer
 * 					la alarma.
 * @param retardo Codifica el retardo en milisegundos y el indicador de alarma
 * 				  periódica en el bit más alto.
 *                Si el retardo es cero, la alarma se cancela.
 * @param auxData Campo auxiliar que se encolará en la cola de eventos además
 * 				  del ID_evento.
 */
void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData);

/**
 * Verifica si hay que disparar el evento asociado a alguna de las alarmas
 * programadas pendientes.
 * Esta función comprueba el tiempo transcurrido desde el inicio de cada alarma
 * programada y dispara el evento asociado si ha llegado el momento. Las alarmas
 * no periódicas se cancelan tras dispararse, mientras que las periódicas
 * continúan activas de forma indefinida hasta que se eliminan.
 */
void alarma_tratar_eventos(void);

#endif
