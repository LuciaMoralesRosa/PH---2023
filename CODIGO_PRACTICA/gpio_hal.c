/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				gpio_hal.c
*/
#include "gpio_hal.h"

/**
 * Inicializacion del modulo GPIO
*/
void gpio_hal_iniciar(void) {
		//Los pinsel y sentido para todas las partes de gpio que se usen
}


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
					  enum gpio_hal_pin_dir_t direccion) {
		uint32_t mascara;
		if (direccion == GPIO_HAL_PIN_DIR_INPUT) {
				mascara = ((0x1 << num_bits) - 0x1) << gpio_inicial;
				IODIR &= ~mascara;		// Configurar pines como entradas
				IOCLR = mascara;		// Inicializar las salidas a 0
		}
		else if (direccion == GPIO_HAL_PIN_DIR_OUTPUT) {
				mascara = (0xFFFFFFFF >> (32 - num_bits)) << gpio_inicial;
				IODIR = mascara;		// Configurar pines como salidas
				IOCLR = mascara;		// Inicializar las salidas a 0
		}
}

/**
 * Lee el valor de pines GPIO.
 * Esta función lee el valor de los pines GPIO especificados y devuelve el resultado.
 *
 * @param gpio_inicial Pin GPIO inicial.
 * @param num_bits Número de bits contiguos a leer.
 * @return Valor leído de los pines GPIO.
 */
uint32_t gpio_hal_leer(GPIO_HAL_PIN_T gpio_inicial, uint8_t num_bits) {
		uint32_t mascara = ((0x1 << num_bits) - 0x1) << gpio_inicial;
		return (IOSET & mascara) >> gpio_inicial;
}

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
					   uint32_t valor) {
		uint32_t mascara = ((0x1 << num_bits) - 0x1) << bit_inicial;
		uint32_t masval = (valor << bit_inicial) & mascara;
		IOSET = masval;
		IOCLR = (~(IOSET & masval)) & mascara;
}

