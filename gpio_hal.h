/**
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				gpio_hal.h
*/

#ifndef GPIO_HAL_H
#define GPIO_HAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <LPC210x.H>

enum gpio_hal_pin_dir_t{
	GPIO_HAL_PIN_DIR_INPUT = 0,
	GPIO_HAL_PIN_DIR_OUTPUT = 1,
};

typedef int GPIO_HAL_PIN_T;

// Permite emplear el GPIO y debe ser invocada antes de poder llamar al resto de funciones de la biblioteca
void gpio_hal_iniciar(void);

// Los bits indicados se utilizarán como entrada o salida según la dirección.
void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, enum gpio_hal_pin_dir_t direccion);

// Gpio_inicial indica el primer bit a leer, num_bits indica cuántos bits queremos leer. 
// La función devuelve un entero con el valor de los bits indicados.
uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits);

// Escribe en los bits indicados el valor 
// (si valor no puede representarse en los bits indicados se escribirá los num_bits menos significativos a partir del inicial)
void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor);

#endif
