/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				gpio_hal.h
*/

#ifndef GPIO_HAL_H
#define GPIO_HAL_H

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <LPC210x.H>

// Definicion del enum gpio_hal_pin_dir_t
enum gpio_hal_pin_dir_t{
	GPIO_HAL_PIN_DIR_INPUT = 0,
	GPIO_HAL_PIN_DIR_OUTPUT = 1,
};

// Definicion del tipo de dato GPIO_HAL_PIN_T para mejorar la legibilidad del 
// código.
typedef int GPIO_HAL_PIN_T;

/**
 * Inicializacion del modulo GPIO
 */
void gpio_hal_iniciar(void);

/**
 * Configura el sentido (entrada/salida) de pines GPIO.
 * Esta función configura el sentido de los pines GPIO, estableciendo si deben
 * ser utilizados como entrada o salida.
 *
 * @param gpio_inicial Pin GPIO inicial.
 * @param num_bits Número de bits contiguos a configurar.
 * @param direccion Dirección deseada para los pines GPIO (entrada o salida).
 */
void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits,
					  enum gpio_hal_pin_dir_t direccion);

/**
 * Lee el valor de pines GPIO.
 * Esta función lee el valor de los pines GPIO especificados y devuelve el resultado.
 *
 * @param gpio_inicial Pin GPIO inicial.
 * @param num_bits Número de bits contiguos a leer.
 * @return Valor leído de los pines GPIO.
 */
uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits);

/**
 * Escribe un valor en los bits especificados.
 * Esta función escribe el valor proporcionado en los bits especificados,
 * considerando el bit inicial y el número de bits indicados. Si el valor no
 * puede representarse en los bits especificados, se escriben los bits menos
 * significativos a partir del bit inicial.
 *
 * @param bit_inicial Bit GPIO inicial.
 * @param num_bits Número de bits contiguos a escribir.
 * @param valor Valor a escribir en los bits especificados.
 */
void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits,
					   uint32_t valor);

#endif
