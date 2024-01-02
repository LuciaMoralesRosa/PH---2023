/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				temporizador_hal.c
 */
#include "temporizador_hal.h"

static volatile unsigned int temporizador_int_count = 0;
void (*ptr)();

/**
 * Maneja la interrupción del Timer0.
 * Incrementa el contador de interrupciones y limpia la bandera de interrupción.
 */
void timer0_ISR(void) __irq {
		temporizador_int_count++;
		T0IR = 1;				// Clear interrupt flag
		VICVectAddr = 0;		// Marcar como interrupcion atendida
}

/**
 * Programa un contador para ser utilizado.
 */
void temporizador_hal_iniciar(void) {
		// Configuracion del Timer0
		T0MR0 = 0xFFFFFFFF;		// Interrumpe cada 0,001ms -> 5 mins = 5*60 secs = 300000 ms
		T0MCR = 3;				// Genera una interrupcion y resetea la cuenta cuando llega al valor de MR0
		T0TCR = 1;				// Temporizador Enable
		//Configuracion de la funcion que se ejecutara cuando llegue una interrupcion
		VICVectAddr0 = (unsigned long)timer0_ISR;
}

/**
 * Inicia el temporizador y habilita las interrupciones asociadas.
 */
void temporizador_hal_empezar(void) {
		temporizador_int_count = 0;	
		VICVectCntl0 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010; // Habilita las interrupciones del temporizador
}

/**
 * Lee el tiempo que lleva contando el contador desde que se ejecutó
 * temporizador_hal_empezar() y lo devuelve en microsegundos.
 * @return Tiempo transcurrido en microsegundos.
 */
uint64_t temporizador_hal_leer(void) {
	return T0TC + (T0MR0 * temporizador_int_count);
}

/**
 * Detiene el temporizador y devuelve el tiempo transcurrido desde el último 
 * temporizador_hal_empezar().
 * @return Tiempo transcurrido en microsegundos.
 */
uint64_t temporizador_hal_parar(void){
		VICVectCntl0 = 0x20 | 4;
		// Deshabilita las interrupciones del temporizador
		VICIntEnClr = VICIntEnClr | 0x00000010;	
		return T0TC + (T0MR0 * temporizador_int_count);
}

/**
 * Maneja la interrupción del Timer1.
 */
void timer1_ISR() __irq {
		(*ptr)(); 			// Se ejecuta la funcion guardada
		T1IR = 1;
		VICVectAddr = 0;	// Marcar como interrupcion atendida
}

/** 
 * Detiene el contador del Timer1
 */
void temporizador_hal_parar_timer1(void){
		VICVectCntl0 = 0x20 | 5; 
		// Deshabilita las interrupciones del temporizador
		VICIntEnClr = VICIntEnClr | 0x00000020;	
}

/**
 * Programa el reloj para que llame a la función de callback cada periodo.
 * Si el periodo es cero, se para el temporizador.
 */
void temporizador_hal_reloj(uint32_t periodo, void (*funcion_callback)()) {
		// Configuracion del Timer1
		T1MR0 = periodo - 1;	// Interrumpe cada periodo milisegundos
		T1MCR = 3;				// Genera una interrupcion y resetea la cuenta 
								// cuando llega al valor de MR0
    	T1TCR = 1;				// Habilita el temporizador

		if(periodo == 0){ 		//Parar el temporizador
				temporizador_hal_parar_timer1();
		}
		else { 					//Llamar a la funcion callback
				ptr = funcion_callback;
				//Configuracion de la funcion que se ejecutara cuando llegue una interrupcion
				VICVectAddr0 = (unsigned long)timer1_ISR;
				VICVectCntl0 = 0x20 | 5;
				// Habilitar interrupciones del tempoporizador
				VICIntEnable = VICIntEnable | 0x00000020; 
		}
}
