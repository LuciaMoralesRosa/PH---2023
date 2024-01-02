/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				int_externas_hal.c
 */

#include "int_externas_hal.h"

void (*callback_pulsado)();

/**
 * Inicializa las interrupciones externas.
 * Esta función se encarga de inicializar las interrupciones externas,
 * estableciendo la función de retorno de llamada para el tratamiento de las
 * interrupciones de pulsado.
 *
 * @param funcion_callback Función de retorno de llamada para el tratamiento de
 * 						   las interrupciones de pulsado.
 */
void int_externas_hal_init(void (*funcion_callback)()) {
		callback_pulsado = funcion_callback;
}

/**
 * Rutina de Servicio de Interrupción (ISR) para EINT1.
 * Esta función se ejecuta en respuesta a una interrupción de EINT1. Limpia la
 * pulsación previa, marca la interrupción como manejada y desactiva la
 * interrupción de EINT1. Luego, invoca la función de retorno de llamada para el
 * tratamiento de la pulsación.
 */
void eint1_ISR(void) __irq {
		// Limpiar pulsacion previa
		EXTINT =  EXTINT | 2;
		// Marcar la interrupcion como manejada
		VICVectAddr = 0;        

		// Iniciar deteccion de boton liberado desactivando la interrupcion de EXTINT1
		VICIntEnClr = VICIntEnClr | 0x00008000;		

		// Invocar la función de retorno de llamada para el tratamiento de la pulsación
		callback_pulsado(EINT1);
}

/**
 * Inicializa la configuración para EINT1.
 * Esta función se encarga de realizar la configuración inicial para el manejo de la
 * interrupción externa EINT1. Limpia la pulsación previa, configura la llamada a la
 * función de servicio de interrupción (__irq) correspondiente (eint1_ISR), prepara
 * el pin como botón y activa las interrupciones para EINT1.
 */
void eint1_init(void) {
		// Limpiar pulsacion previa
		EXTINT =  EXTINT | 2;		
		
		// Configurar llamada a funcion __irq cuando se pulse eint1
		VICVectAddr1 = (unsigned long)eint1_ISR;

		// Preparar PINSEL como boton
		PINSEL0 = PINSEL0 & 0xCFFFFFFF; 		  // Limpia bits 28-29 de PINSEL0
		PINSEL0 = PINSEL0 | 0x20000000;			  // Activa bits PINSEL0

		// Activación de interrupciones EINT1
		VICVectCntl1 = 0x20 | 15;				  // 15 es el EINT1
		VICIntEnable = VICIntEnable | 0x00008000; // Permite interrupcion de EXTINT1
}

/**
 * Rutina de Servicio de Interrupción (ISR) para EINT2.
 * Esta función se ejecuta en respuesta a una interrupción de EINT2. Limpia la
 * pulsación previa, marca la interrupción como manejada y desactiva la
 * interrupción de EINT2. Luego, invoca la función de retorno de llamada para el
 * tratamiento de la pulsación.
 */
void eint2_ISR(void) __irq {
		// Limpiar pulsacion previa
		EXTINT =  EXTINT | 4;
		// Marcar la interrupcion como manejada
		VICVectAddr = 0;     
		// Iniciar deteccion de boton liberado

		// Iniciar deteccion de boton liberado desactivando la interrupcion de EXTINT2
		VICIntEnClr = VICIntEnClr | 0x00010000;

		// Invocar la función de retorno de llamada para el tratamiento de la pulsación
		callback_pulsado(EINT2);
}

/**
 * Inicializa la configuración para EINT2.
 * Esta función se encarga de realizar la configuración inicial para el manejo de la
 * interrupción externa EINT2. Limpia la pulsación previa, configura la llamada a la
 * función de servicio de interrupción (__irq) correspondiente (eint1_ISR), prepara
 * el pin como botón y activa las interrupciones para EINT1.
 */
void eint2_init(void) {
		// Limpiar pulsacion previa
		EXTINT =  EXTINT | 4;		
		
		// Configurar llamada a funcion __irq cuando se pulse eint2
		VICVectAddr2 = (unsigned long)eint2_ISR;

		// Preparar PINSEL como boton
		PINSEL0 		= PINSEL0 & 0x3FFFFFFF;		// Limpia bits 30-31 de PINSEL0
		PINSEL0 		= PINSEL0 | 0x80000000;		// Activa bits PINSEL0
		
		// Activacion de interrupciones EINT2
		VICVectCntl2 = 0x20 | 16;					// 16 es el EINT2
		VICIntEnable = VICIntEnable | 0x00010000;	// Permite interrupcion de EXTINT2
}

/**
 * Maneja la interrupción de EINT1 y la reactiva.
 *
 * Esta función se encarga de manejar la interrupción de EINT1, limpiando la
 * pulsación previa y reactivando la interrupción de EINT1 para permitir futuras
 * pulsaciones.
 */
void eint1_interrupcion_manejada(void) {
		// Limpia pulsacion previa
		EXTINT =  EXTINT | 2;
		// Reactiva interrupcion de EXTINT1
		VICIntEnable = VICIntEnable | 0x00008000;
}

/**
 * Maneja la interrupción de EINT2 y la reactiva.
 *
 * Esta función se encarga de manejar la interrupción de EINT2, limpiando la
 * pulsación previa y reactivando la interrupción de EINT2 para permitir futuras
 * pulsaciones.
 */
void eint2_interrupcion_manejada(void) {
		// Limpia pulsacion previa
		EXTINT =  EXTINT | 4;			
		// Reactiva interrupcion de EXTINT2
		VICIntEnable = VICIntEnable | 0x00010000;		
}

/**
 * Verifica si EINT1 está actualmente pulsado.
 * Esta función se encarga de verificar si el botón asociado a EINT1 está
 * actualmente pulsado. Limpia la pulsación previa y devuelve un valor booleano
 * indicando si la pulsación sigue activa.
 *
 * @return Devuelve 1 si la pulsación sigue activa (botón pulsado) y 0 en caso
 * contrario.
 */
uint8_t eint1_esta_pulsado(void) {
		// Limpia pulsacion previa
		EXTINT =  EXTINT | 2;
		// Devuelve si la pulsacion sigue activa, lo que solo pasa si el boton sigue pulsado
		return (EXTINT >> 1) & 1; 
}

/**
 * Verifica si EINT2 está actualmente pulsado.
 * Esta función se encarga de verificar si el botón asociado a EINT2 está
 * actualmente pulsado. Limpia la pulsación previa y devuelve un valor booleano
 * indicando si la pulsación sigue activa.
 *
 * @return Devuelve 1 si la pulsación sigue activa (botón pulsado) y 0 en caso
 * contrario.
 */
uint8_t eint2_esta_pulsado(void) {
		// Limpia pulsacion previa
		EXTINT =  EXTINT | 4;
		// Devuelve si la pulsacion sigue activa, lo que solo pasa si el boton sigue pulsado
		return (EXTINT >> 2) & 1; 
}
