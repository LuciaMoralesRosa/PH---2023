/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				botones.h
 */

#include "botones.h"

// Definicion de variables
void (*funcion_encolar)();
static int USUARIO_AUSENTE;
static enum EVENTO_T ev_PULSAR;
static enum EVENTO_T ev_MONITORIZAR;
static enum EVENTO_T ev_USER_AUSENTE;

// FUNCIONES PRIVADAS

/**
 * Llama a la función de encolado de eventos para el evento de pulsar un botón.
 * Esta función convierte el enumerador de botones en un valor auxiliar y llama
 * a la función de encolado de eventos con el evento correspondiente y el valor
 * auxiliar.
 *
 * @param boton Botón pulsado.
 */
void funcion_llamar_encolar_botones(enum BOTON boton){
		uint32_t aux = boton;
		funcion_encolar(ev_PULSAR, aux);
}

// FUNCIONES PUBLICAS

/**
 * Inicializa el subsistema de botones.
 * Esta función realiza la inicialización del subsistema de botones,
 * configurando las interrupciones externas, estableciendo la función de
 * callback, y asignando eventos para pulsar, monitorizar y detectar ausencia
 * de usuario.
 *
 * @param funcion_callback Puntero a la función de callback que se ejecutará al
 * 						   pulsar un botón.
 * @param usuario_ausente Valor que indica si el usuario está ausente.
 * @param pulsado Evento asociado al pulsar un botón.
 * @param monitorizar Evento asociado a la monitorización de botones.
 * @param ausente Evento asociado a la detección de ausencia de usuario.
 */
void boton_init(void (*funcion_callback)(), int usuario_ausente,
					  enum EVENTO_T pulsado, enum EVENTO_T monitorizar,
					  enum EVENTO_T ausente){
		int_externas_hal_init(boton_ha_sido_pulsado);
		funcion_encolar = funcion_callback;
		ev_PULSAR = pulsado;
		ev_MONITORIZAR = monitorizar;
		ev_USER_AUSENTE = ausente;
		eint1_init();
		eint2_init();
		USUARIO_AUSENTE = usuario_ausente;
}

/**
 * Gestiona las interrupciones de botones.
 * Esta función se encarga de gestionar las interrupciones generadas por los
 * botones. Comprueba el tipo de botón que generó la interrupción y realiza
 * acciones correspondientes, como cancelar una alarma y manejar la interrupción
 * del botón pulsado.
 *
 * @param auxData Valor auxiliar que indica el tipo de botón que generó la
 * 				  interrupción (EINT1 o EINT2).
 */
void boton_gestion(uint32_t auxData){
		switch (auxData) {
				case EINT1:
						if (eint1_esta_pulsado()==0) {
							eint1_interrupcion_manejada();
							// Cancelar alarma
							alarma_activar(ev_MONITORIZAR, 0, EINT1); 
						} 
						break;
				case EINT2:
						if (eint2_esta_pulsado()==0) {
							eint2_interrupcion_manejada();
							// Cancelar alarma
							alarma_activar(ev_MONITORIZAR, 0, EINT2); 
						} 
						break;
				default:
						while(1); //Error
		}
}

/**
 * Maneja el evento de pulsar un botón.
 * Esta función es llamada cuando un botón ha sido pulsado. Encola un evento de
 * pulsado para que el planificador ejecute lo que tenga que ejecutar cuando el
 * botón se presiona por primera vez.
 * Inicia una alarma que causará que el planificador avise cuando haya que
 * verificar si el botón se ha liberado.
 *
 * @param boton Botón que ha sido pulsado.
 */
void boton_ha_sido_pulsado(enum BOTON boton){
		funcion_llamar_encolar_botones(boton); //Encola evento de pulsado
		alarma_activar(ev_MONITORIZAR, 0x80000064, boton);
		alarma_activar(ev_USER_AUSENTE, USUARIO_AUSENTE, 0);
}
