/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				hello_world.c
*/

#include "hello_world.h"

static unsigned int G_Hello_World = 0;
static unsigned int G_Hello_World_Bits = 0;

//Inicializa G_Hello_World y G_Hello_World_Bits
void hello_world_inicializar(int GPIO_HELLO_WORD, int GPIO_HELLO_WORD_BITS){
	G_Hello_World = GPIO_HELLO_WORD;
	G_Hello_World_Bits = GPIO_HELLO_WORD_BITS;
	gpio_hal_sentido(G_Hello_World, G_Hello_World_Bits, GPIO_HAL_PIN_DIR_INPUT);
	gpio_hal_escribir(G_Hello_World, G_Hello_World_Bits, 0);
}

//Actualiza el contador y el estado de los leds 
void hello_world_tick_tack(void){
	uint32_t valor;	
	valor = gpio_hal_leer(G_Hello_World, G_Hello_World_Bits);
	valor++;
	gpio_hal_sentido(G_Hello_World, G_Hello_World_Bits, GPIO_HAL_PIN_DIR_INPUT); //Sobrara -> poner en gpio inicializar
	gpio_hal_escribir(G_Hello_World, G_Hello_World_Bits, valor);
}
