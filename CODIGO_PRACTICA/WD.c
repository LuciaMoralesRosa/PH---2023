/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				watchdog.c
*/

#include "WD.h"

/**
 * Inicializa el Watchdog Timer para que resetee el procesador dentro de sec
 * segundos si no se le "alimenta".
 * Esta función configura el Watchdog Timer para que reinicie el procesador si
 * no se le "alimenta" dentro del tiempo especificado. Se asigna el tiempo de
 * cuenta con el registro WDTC, y se activan los bits de reset y enable en el 
 * registro WDMOD.
 *
 * @param sec El tiempo en segundos antes de que el Watchdog Timer reinicie el procesador.
 */
void WD_hal_inicializar(int sec){
		//Asignar tiempo de cuenta con el registro WDTC
		int producto = 15000 * 256 * 4;
		int valor = sec * producto;
		if(valor < producto){
			valor = producto;
		}
		WDTC = valor;
		//Activar bit de reset y de enable en el registro WDMOD
		WDMOD = 0x3;
		
		WD_hal_feed();
}

/**
 * Alimenta al Watchdog Timer.
 * Esta función alimenta el Watchdog Timer para evitar que alcance su tiempo de
 * espera y reinicie el procesador. Se desactivan temporalmente las
 * interrupciones durante las llamadas consecutivas para garantizar la
 * secuencia correcta.
 */
void WD_hal_feed(void) {
	// Las llamadas deben ser consecutivas, apagamos interrupciones
	uint32_t read_bit;
	read_bit = read_IRQ_bit();
	disable_irq();
	WDFEED = 0xAA;
	WDFEED = 0x55;
	// Rehabilitamos las interrupciones
	if (read_bit) {
		enable_irq();
	}
}

/**
 * Test con un bucle infinito para comprobar que el Watchdog funciona.
 */
void WD_hal_test(void){
	while (1){}
}
