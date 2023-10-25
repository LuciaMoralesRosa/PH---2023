/**
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				planificador.c
*/

#include "planificador.h"

//Muestra overflow en el GPIO de overflow y parará la ejecución
void gestionar_alarma_overflow(void){
	gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 0x1); // Encender el LED
	timer0_drv_parar();	
}


//inicializa la cola y se queda en un blucle infinito pendiente de eventos
void planificador(void){
	BOOLEANO hay_eventos = true;
	enum EVENTO_T id_eventos;
	uint32_t aux_data;
	FIFO_inicializar(GPIO_OVERFLOW);
	alarma_inicializar();
	hello_world_inicializar(GPIO_HELLO_WORD, GPIO_HELLO_WORD_BITS);
	timer1_drv_reloj(1, hello_world_tick_tack);
	while(1){
		//Gestion de eventos
		if(no_hay_alarmas() && esta_vacia()){
			power_hal_wait();
		}
		else{
			alarma_tratar_eventos();
			while(hay_eventos){
				hay_eventos = FIFO_extraer(&id_eventos, &aux_data);
				switch(id_eventos){
					case ALARMA_OVERFLOW:
						gestionar_alarma_overflow();
				}
			}
			
		}
	}
}



//EVENTOS A GESTIONAR ___________________________________________________________

//ALARMA_OVERFLOW: ------------
//Cuando gestione el evento ALARMA_OVERFLOW:
	//el planificador lo mostrará en el GPIO de overflow y parará la ejecución

