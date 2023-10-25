/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				int_externas_hal.c
 */

#include "int_externas_hal.h"

// mirar el si esta pulsado o no

// Interrupcion de eint1
void eint1_ISR (void) __irq {
	
	EXTINT =  EXTINT | 1;		// Limpiar pulsacion previa para que deje de interrumpir
	VICVectAddr = 0;        // Marcar la interrupcion como manejada
	
	// Iniciar deteccion de boton liberado
	VICIntEnClr = VICIntEnClr | 0x00008000;		// Desactiva interrupcion de EXTINT1
}

// Inicializa eint1
void eint1_init (void) {
	EXTINT =  EXTINT | 1;		// Limpiar pulsacion previa
	
	// Configurar llamada a funcion __irq cuando se pulse eint1
	VICVectAddr1 = (unsigned long)eint1_ISR;

	// Preparar PINSEL como botón
	PINSEL0 		= PINSEL0 & 0xCFFFFFFF;					// Limpia bits 28-29 de PINSEL0
	PINSEL0 		= PINSEL0 | 0x20000000;					// Activa bits PINSEL0
	
	// Activación de interrupciones eint1
	VICVectCntl1 = 0x20 | 15;										// 15 es el EINT1
  VICIntEnable = VICIntEnable | 0x00008000;		// Permite interrupcion de EXTINT1
}

// Interrupcion de eint2
void eint2_ISR (void) __irq {
	
	EXTINT =  EXTINT | 1;		// Limpiar pulsacion previa para que deje de interrumpir
	VICVectAddr = 0;        // Marcar la interrupcion como manejada
	// Iniciar deteccion de boton liberado
	VICIntEnClr = VICIntEnClr | 0x00010000;		// Desactiva interrupcion de EXTINT2
}

// Inicializa eint2
void eint2_init (void) {
	EXTINT =  EXTINT | 1;		// Limpiar pulsacion previa
	
	// Configurar llamada a funcion __irq cuando se pulse eint2
	VICVectAddr2 = (unsigned long)eint2_ISR;

	// Preparar PINSEL como botón
	PINSEL0 		= PINSEL0 & 0x3FFFFFFF;					// Limpia bits 30-31 de PINSEL0
	PINSEL0 		= PINSEL0 | 0x80000000;					// Activa bits PINSEL0
	
	// Activación de interrupciones eint2
	VICVectCntl2 = 0x20 | 16;										// 16 es el EINT2
  VICIntEnable = VICIntEnable | 0x00010000;		// Permite interrupcion de EXTINT2
}

// Reactiva las interrupciones eint1
void eint1_interrupcion_manejada(void) {
	VICIntEnable = VICIntEnable | 0x00008000;		// Reactiva interrupcion de EXTINT1
}

// Reactiva las interrupciones eint2
void eint2_interrupcion_manejada(void) {
	VICIntEnable = VICIntEnable | 0x00010000;		// Reactiva interrupcion de EXTINT2
}
