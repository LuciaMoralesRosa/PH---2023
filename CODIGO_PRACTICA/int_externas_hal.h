/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				int_externas_hal.h
 */

#ifndef INT_EXTERNAS_HAL_H
#define INT_EXTERNAS_HAL_H

// Includes
#include <LPC210x.H>
#include <stdint.h>

// Modulos empleados
#include "botones.h"

/**
 * Inicializa las interrupciones externas.
 * Esta función se encarga de inicializar las interrupciones externas,
 * estableciendo la función de retorno de llamada para el tratamiento de las
 * interrupciones de pulsado.
 *
 * @param funcion_callback Función de retorno de llamada para el tratamiento de
 * 						   las interrupciones de pulsado.
 */
void int_externas_hal_init(void (*funcion_callback)());

/**
 * Inicializa la configuración para EINT1.
 * Esta función se encarga de realizar la configuración inicial para el manejo de la
 * interrupción externa EINT1. Limpia la pulsación previa, configura la llamada a la
 * función de servicio de interrupción (__irq) correspondiente (eint1_ISR), prepara
 * el pin como botón y activa las interrupciones para EINT1.
 */
void eint1_init(void);

/**
 * Inicializa la configuración para EINT2.
 * Esta función se encarga de realizar la configuración inicial para el manejo de la
 * interrupción externa EINT2. Limpia la pulsación previa, configura la llamada a la
 * función de servicio de interrupción (__irq) correspondiente (eint1_ISR), prepara
 * el pin como botón y activa las interrupciones para EINT1.
 */
void eint2_init(void);

/**
 * Maneja la interrupción de EINT1 y la reactiva.
 *
 * Esta función se encarga de manejar la interrupción de EINT1, limpiando la
 * pulsación previa y reactivando la interrupción de EINT1 para permitir futuras
 * pulsaciones.
 */
void eint1_interrupcion_manejada(void);

/**
 * Maneja la interrupción de EINT2 y la reactiva.
 *
 * Esta función se encarga de manejar la interrupción de EINT2, limpiando la
 * pulsación previa y reactivando la interrupción de EINT2 para permitir futuras
 * pulsaciones.
 */
void eint2_interrupcion_manejada(void);

/**
 * Verifica si EINT1 está actualmente pulsado.
 * Esta función se encarga de verificar si el botón asociado a EINT1 está
 * actualmente pulsado. Limpia la pulsación previa y devuelve un valor booleano
 * indicando si la pulsación sigue activa.
 *
 * @return Devuelve 1 si la pulsación sigue activa (botón pulsado) y 0 en caso
 * contrario.
 */
uint8_t eint1_esta_pulsado(void);

/**
 * Verifica si EINT2 está actualmente pulsado.
 * Esta función se encarga de verificar si el botón asociado a EINT2 está
 * actualmente pulsado. Limpia la pulsación previa y devuelve un valor booleano
 * indicando si la pulsación sigue activa.
 *
 * @return Devuelve 1 si la pulsación sigue activa (botón pulsado) y 0 en caso
 * contrario.
 */
uint8_t eint2_esta_pulsado(void);

#endif
