/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				timer0_drv.c
 */
#include "temporizador_drv.h"

// Es volatile para que se sepa que cambia
//static volatile unsigned int timer0_int_count = 0;

// Programa un contador para que pueda ser utilizado.
void timer0_drv_iniciar(void) {
		timer0_hal_iniciar();
}

// Inicia la cuenta de un contador de forma indefinida.
void timer0_drv_empezar(void) {
		timer0_hal_empezar();
}

// Lee el tiempo que lleva contando el contador desde que se ejecutó
// temporizador_hal_empezar() y lo devuelve en milisegundos
uint64_t timer0_drv_leer(void) {
	return timer0_hal_leer()/timer0_hal_ticks2us;
}

// Detiene el contador y devuelve el tiempo en milisegundos transcurrido desde
// el último temporizador_hal_empezar()
uint64_t timer0_drv_parar(void){
		return timer0_hal_parar();
}

void timer1_drv_reloj(uint32_t periodo, void (*funcion_callback)()){
		timer1_hal_reloj(periodo*timer0_hal_ticks2us, funcion_callback);
}
