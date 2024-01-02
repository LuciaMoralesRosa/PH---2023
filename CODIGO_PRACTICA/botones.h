/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				botones.h
 */

#ifndef BOTONES_H
#define BOTONES_H

//Includes
#include <stdint.h>

//Modulos empleados
#include "int_externas_hal.h"
#include "alarma.h"

// DEfinicion de los valores correspondientes a los botones del sistema
enum BOTON {
	EINT1 = 15,
	EINT2 = 16
};

/**
 * Inicializa el subsistema de botones.
 * Esta función realiza la inicialización del subsistema de botones,
 * configurando las interrupciones externas, estableciendo la función de
 * callback, y asignando eventos para pulsar, monitorizar y detectar ausencia
 * de usuario.
 *
 * @param funcion_callback Puntero a la función de callback que se ejecutará al
 * 						   pulsar un botón.
 * @param usuario_ausente Valor que indica si el usuario está ausente.
 * @param pulsado Evento asociado al pulsar un botón.
 * @param monitorizar Evento asociado a la monitorización de botones.
 * @param ausente Evento asociado a la detección de ausencia de usuario.
 */
void boton_init(void (*funcion_callback)(), int usuario_ausente,
					  enum EVENTO_T pulsado, enum EVENTO_T monitorizar,
					  enum EVENTO_T ausente);

/**
 * Gestiona las interrupciones de botones.
 * Esta función se encarga de gestionar las interrupciones generadas por los
 * botones. Comprueba el tipo de botón que generó la interrupción y realiza
 * acciones correspondientes, como cancelar una alarma y manejar la interrupción
 * del botón pulsado.
 *
 * @param auxData Valor auxiliar que indica el tipo de botón que generó la
 * 				  interrupción (EINT1 o EINT2).
 */
void boton_gestion(uint32_t auxData);

/**
 * Maneja el evento de pulsar un botón.
 * Esta función es llamada cuando un botón ha sido pulsado. Encola un evento de
 * pulsado para que el planificador ejecute lo que tenga que ejecutar cuando el
 * botón se presiona por primera vez.
 * Inicia una alarma que causará que el planificador avise cuando haya que
 * verificar si el botón se ha liberado.
 *
 * @param boton Botón que ha sido pulsado.
 */
void boton_ha_sido_pulsado(enum BOTON boton);

#endif
