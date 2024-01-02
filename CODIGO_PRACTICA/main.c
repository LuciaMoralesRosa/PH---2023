/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				main.c
 */

#include <LPC210x.H>                       /* LPC210x definitions */
#include "planificador.h"

int main(void){

	//Inicializar modulos
	disable_fiq();
	planificador();
	
}
