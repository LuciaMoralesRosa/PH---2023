/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				linea_serie_hal.h
*/

#ifndef LINEA_SERIE_HAL
#define LINEA_SERIE_HAL

// Includes
#include <LPC210x.H>  

// Modulos empleados
#include "swi.h"
#include "eventos.h"
#include "gpio_hal.h"

/**
 * Inicializa el módulo de la línea serie (UART0) y registra funciones de callback.
 * Esta función realiza la configuración inicial del módulo de la línea serie
 * (UART0) y registra las funciones de encolar, callback y envío proporcionadas
 * como parámetros. Configura los pines de la línea serie, los registros de
 * control de línea (U0LCR), el divisor de la línea (U0DLL), y habilita las
 * interrupciones necesarias. Además, registra las funciones de encolar,
 * callback y envío para su uso posterior en las rutinas de interrupción.
 *
 * @param funcion_encolar Puntero a la función de encolar eventos.
 * @param funcion_callback Puntero a la función de callback para manejar
 * 						   caracteres recibidos.
 * @param funcion_envio Puntero a la función de continuación de envío.
 */
void inicializar_linea_serie_hal(void (*funcion_encolar)(),
										void (*funcion_callback)(),
										void (*funcion_envio)());

/**
 * Envia un carácter a través de la línea serie (UART0).
 * Esta función envía el carácter proporcionado como parámetro a través del registro
 * de transmisión THR (U0THR) de la línea serie (UART0). El carácter se transmitirá
 * tan pronto como sea posible.
 *
 * @param caracter El carácter a enviar a través de la línea serie.
 */
void linea_serie_hal_enviar_caracter(char caracter);
#endif
