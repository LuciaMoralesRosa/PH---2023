/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				temporizador_drv.c
 */
#include "temporizador_drv.h"

static enum EVENTO_T evento;
static void (*funcion_auxiliar)();

/**
 * Inicia el temporizador.
 * Esta función invoca la función de nivel inferior (temporizador_hal_iniciar())
 * para iniciar el temporizador. La implementación específica del hardware se
 * encargará de las configuraciones necesarias para poner en marcha el 
 * temporizador.
 */
void temporizador_drv_iniciar(void) {
	temporizador_hal_iniciar();
}

/**
 * Empieza la operación del temporizador.
 * Esta función invoca la función de nivel inferior (temporizador_hal_empezar())
 * para comenzar la operación del temporizador. La implementación específica del
 * hardware se encargará de las acciones necesarias para iniciar la cuenta del
 * tiempo.
 */
void temporizador_drv_empezar(void) {
	temporizador_hal_empezar();
}

/**
 * Lee el tiempo transcurrido desde que se inició el temporizador.
 * Esta función invoca la función de nivel inferior (temporizador_hal_leer()) para
 * obtener el tiempo contado por el temporizador en unidades de ticks. Luego,
 * realiza la conversión a milisegundos dividiendo por la constante `temporizador_hal_ticks2us`.
 *
 * @return El tiempo transcurrido en milisegundos desde que se inició el temporizador.
 */
uint64_t temporizador_drv_leer(void) {
	return temporizador_hal_leer() / temporizador_hal_ticks2us;
}

/**
 * Llamada SWI para acceder a temporizador_drv_leer desde otros módulos.
 * Esta función realiza una llamada de Software Interrupt (SWI) para acceder a la
 * función `temporizador_drv_leer` desde otros módulos. La implementación específica
 * del sistema embebido debe tener una rutina de manejo de SWI que se asocie con este
 * número específico de SWI (en este caso, SWI número 0).
 *
 * @return El tiempo transcurrido en milisegundos desde que se inició el temporizador.
 */
uint32_t __SWI_0 (void) {
	return temporizador_drv_leer();
}

/**
 * Detiene el contador y devuelve el tiempo transcurrido desde la última
 * temporizador_hal_empezar().
 * Esta función invoca la función de nivel inferior (temporizador_hal_parar()) para
 * detener el temporizador y obtener el tiempo transcurrido desde el último inicio. 
 * El tiempo se devuelve en milisegundos.
 *
 * @return El tiempo transcurrido en milisegundos desde el último temporizador_hal_empezar().
 */
uint64_t temporizador_drv_parar(void){
		return temporizador_hal_parar();
}

/**
 * Función auxiliar que llama a la función_auxiliar con el evento actual.
 * Esta función sirve como un puente para llamar a la función_auxiliar con el
 * evento actual y un valor auxiliar de 0. Puede ser utilizada como una interfaz
 * de llamada más simple en ciertos contextos.
 */
void funcion_llamar() {
	funcion_auxiliar(evento, 0);
}

/**
 * Programa el reloj para que encole un evento periódicamente en la cola del planificador.
 * Esta función programa el temporizador para que genere eventos periódicos en
 * la cola del planificador. Se especifica el periodo en microsegundos, la
 * función que debe encolar el evento y el ID del evento a ser encolado. La
 * función utiliza una función auxiliar y un evento almacenados globalmente.
 *
 * @param periodo El periodo en microsegundos para la generación periódica de eventos.
 * @param funcion_encolar_evento La función que encolará el evento en la cola del planificador.
 * @param ID_Evento El ID del evento a ser encolado periódicamente.
 */
void temporizador_drv_reloj(uint32_t periodo, void (*funcion_encolar_evento)(),
							enum EVENTO_T ID_Evento){
	funcion_auxiliar = funcion_encolar_evento;
	evento = ID_Evento;
	temporizador_hal_reloj(periodo*temporizador_hal_ticks2us, funcion_llamar);
}
