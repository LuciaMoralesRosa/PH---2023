/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				visualizar.c
 */

#include "visualizar.h"

/**
 * Inicializa la visualización.
 * Esta función inicializa la visualización configurando el sentido del pin de
 * salida y estableciendo su valor inicial en 0.
 */
void visualizar_inicializar(void){
	gpio_hal_sentido(GPIO_VISUALIZAR, GPIO_VISUALIZAR_BITS, GPIO_HAL_PIN_DIR_INPUT);
	gpio_hal_escribir(GPIO_VISUALIZAR, GPIO_VISUALIZAR_BITS, 0);
}

/**
 * Visualiza un contador en la pantalla.
 *
 * @param cuenta El valor a mostrar en la pantalla.
 */
void visualizar_cuenta(uint32_t cuenta){
	gpio_hal_escribir(GPIO_VISUALIZAR, GPIO_VISUALIZAR_BITS, cuenta);
}

/**
 * Incrementa el valor visualizado en la pantalla de saludo.
 * Lee el valor actual del pin de la pantalla de saludo, incrementa su valor y
 * lo escribe de nuevo en el pin.
 */
void visualizar_hello(void){
	uint32_t valor;	
	valor = gpio_hal_leer(GPIO_HELLO_WORD, GPIO_HELLO_WORD_BITS);
	valor++;
	gpio_hal_escribir(GPIO_HELLO_WORD, GPIO_HELLO_WORD_BITS, valor);

}

