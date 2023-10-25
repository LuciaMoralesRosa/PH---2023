/**
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				planificador.c
*/

#include "planificador.h"

//inicializa la cola y se queda en un blucle infinito pendiente de eventos
void planificador(void){
	FIFO_inicializar(0);
	hello_world_inicializar(GPIO_HELLO_WORD, GPIO_HELLO_WORD_BITS);
	timer1_drv_reloj(1, hello_world_tick_tack);
	while(1){
		//Gestion de eventos
	}
	/*Reducción de consumo
	if(esta_vacia() == 1){
		power_hal_wait();
	}*/
}

