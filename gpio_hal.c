/**
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				gpio_hal.c
*/
#include "gpio_hal.h"

// Permite emplear el GPIO y debe ser invocada antes de poder llamar al resto de funciones de la biblioteca
void gpio_hal_iniciar(void) {
	//Los pinsel y sentido para todas las partes de gpio que se usen
}

// Marcar los bits indicados como entrada o salida según la dirección.
void gpio_hal_sentido(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits, enum gpio_hal_pin_dir_t direccion) {
		uint32_t mascara;
		if (direccion == GPIO_HAL_PIN_DIR_INPUT) {
				mascara = ((0x1 << num_bits) - 0x1) << gpio_inicial;
				IODIR		&= ~mascara;					//Set LED pins as outputs
				IOCLR 	= mascara;					//Initialices the outputs to 0
		}
		else if (direccion == GPIO_HAL_PIN_DIR_OUTPUT) {
				mascara = (0xFFFFFFFF >> (32 - num_bits)) << gpio_inicial;
				IODIR		= mascara;					//Set LED pins as outputs
				IOCLR 	= mascara;					//Initialices the outputs to 0
		}
}

// Gpio_inicial indica el primer bit a leer, num_bits indica cuántos bits queremos leer. 
// La función devuelve un entero con el valor de los bits indicados.
uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
	uint32_t mascara = ((0x1 << num_bits) - 0x1) << gpio_inicial;
	return (IOSET & mascara) >> gpio_inicial;
}

// Escribe en los bits indicados el valor 
// (si valor no puede representarse en los bits indicados se escribirá los num_bits menos significativos a partir del inicial)
void gpio_hal_escribir(GPIO_HAL_PIN_T bit_inicial, uint8_t num_bits, uint32_t valor) {
	uint32_t mascara = ((0x1 << num_bits) - 0x1) << bit_inicial;
	uint32_t masval = (valor << bit_inicial) & mascara;
	IOSET = masval;
	IOCLR = (~(IOSET & masval)) & mascara;
}

