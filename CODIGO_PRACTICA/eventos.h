/*
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				eventos.h
 */
 
#ifndef EVENTOS_H
#define EVENTOS_H

//Definicion de la constante NUM_EVENTOS que indica el numero de eventos
#define NUM_EVENTOS 12
 
 //Definicion de los tipos de eventos
enum EVENTO_T {
		ID_VOID = 0, //Inicializar la cola
		ev_TRATAR_EVENTO = 1,
		ev_BOTON_PULSADO = 2,
		ev_BOTON_MONITORIZAR = 3,
		ev_ALARMA_OVERFLOW = 4,
		ev_LATIDO = 5,
		ev_VISUALIZAR_HELLO = 6,
		ev_USUARIO_AUSENTE = 7,
		ev_RX_SERIE = 8,
		ev_TX_SERIE = 9,
		ev_END_GAME = 10,
		ev_RST_CANCELACION= 11
};

#endif
