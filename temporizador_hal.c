/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				temporizador_hal.c
 */
#include "temporizador_hal.h"

// Es volatile para que se sepa que cambia
static volatile unsigned int timer0_int_count = 0;

void (*ptr)();

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer0_ISR (void) __irq {
    timer0_int_count = timer0_int_count + 100000;
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}

// Programa un contador para que pueda ser utilizado.
void timer0_hal_iniciar(void) {
		// configuration of Timer 0
		T0MR0 = 14999;									// Interrumpe cada 0,001ms
		T0MCR = 3;											// Genera una interrupcion y resetea la cuenta cuando llega al valor de MR0
    T0TCR = 1;											// Timer0 Enable
    // configuration of the IRQ slot number 0 of the VIC for Timer 0 Interrupt
		VICVectAddr0 = (unsigned long)timer0_ISR;          // set interrupt vector in 0
}

// Inicia la cuenta de un contador de forma indefinida.
void timer0_hal_empezar(void) {
		timer0_int_count = 0;	
		// 0x20 bit 5 enables vectored IRQs. 
		// 4 is the number of the interrupt assigned. Number 4 is the Timer 0 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010;                  // Enable Timer0 Interrupt
}

// Lee el tiempo que lleva contando el contador desde que se ejecutó
// temporizador_hal_empezar() y lo devuelve en ticks
uint64_t timer0_hal_leer(void) {
	return timer0_int_count;
}

// Detiene el contador y devuelve el tiempo en ticks transcurrido desde
// el último temporizador_hal_empezar()
uint64_t timer0_hal_parar(void){
		VICVectCntl0 = 0x20 | 4;                   
    VICIntEnClr = VICIntEnClr | 0x00000010;                  // Disable Timer0 Interrupt
		return timer0_int_count;
}

/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void timer1_ISR () __irq {
		(*ptr) ();
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                            // Acknowledge Interrupt
}

void timer1_hal_reloj(uint32_t periodo, void (*funcion_callback)()) {
			// configuration of Timer 1
	
		T1MR0 = periodo-1;									// Interrumpe cada periodo
		T1MCR = 3;											// Genera una interrupcion y resetea la cuenta cuando llega al valor de MR0
    T1TCR = 1;											// Timer1 Enable

		ptr = funcion_callback;
		VICVectAddr0 = (unsigned long)timer1_ISR;          // set interrupt vector in 0
		// 0x20 bit 5 enables vectored IRQs. 
		// 5 is the number of the interrupt assigned. Number 5 is the Timer 1 (see table 40 of the LPC2105 user manual  
		VICVectCntl0 = 0x20 | 5;                   
    VICIntEnable = VICIntEnable | 0x00000020;                  // Enable Timer1 Interrupt
}
