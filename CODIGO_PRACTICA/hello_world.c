/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				hello_world.c
*/

#include "hello_world.h"

/**
 * @brief Puntero a función para encolar eventos de "Hello World".
 *
 * Este puntero a función se utiliza para referenciar la función que encola eventos
 * relacionados con la funcionalidad "Hello World". La función de encolado debe tener
 * la firma adecuada para recibir eventos y datos adicionales.
 */
void (*callback_encolar_hello_world)();

/**
 * Inicializa la funcionalidad "Hello World".
 * Esta función configura la funcionalidad "Hello World", activando una alarma
 * con un retardo específico y estableciendo la función de retorno de llamada.
 *
 * @param funcion_callback Función de retorno de llamada para la funcionalidad "Hello World".
 */
void hello_world_inicializar(void (*funcion_callback)()) {
		// Configurar el retardo para la alarma (10 milisegundos con repetición)
		uint32_t retardo = 0x8000000A;

		// Activar la alarma para la funcionalidad "Hello World"
		alarma_activar(ev_LATIDO, retardo, 0);

		// Establecer la función de retorno de llamada
		callback_encolar_hello_world = funcion_callback;
}

/**
 * Trata el evento "Latido" y encola el evento "Visualizar Hello".
 * Esta función se encarga de tratar el evento "Latido" (ev_LATIDO) mediante el
 * alimentado del watchdog y luego encola el evento "Visualizar Hello"
 * (ev_VISUALIZAR_HELLO). La función de retorno de llamada correspondiente a
 * "Hello World" es invocada para realizar las acciones necesarias al tratar el evento.
 */
void hello_world_tratar_evento(void) {
	// Alimentar el Watchdog durante el tratamiento del evento "Latido"
	WD_hal_feed();

	// Encolar el evento "Visualizar Hello"
	callback_encolar_hello_world(ev_VISUALIZAR_HELLO, 0);
}
