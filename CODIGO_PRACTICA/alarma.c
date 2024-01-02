/*
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				alarma.c
*/

#include "alarma.h"

// Definicion de variables
static void (*funcion_encolar)();
static enum EVENTO_T evento;
static uint32_t aux;

// FUNCIONES PRIVADAS

/**
 * Función para llamar y encolar una alarma.
 *
 * @param e Enumeración del tipo de evento a asociar con la alarma.
 * @param a Valor adicional asociado con la alarma.
 *
 */
void funcion_llamar_encolar_alarma(enum EVENTO_T e, uint32_t a) {
		evento = e;
		aux = a;
		funcion_encolar(evento, aux);
}

/**
 * Rellena la estructura de datos de una alarma.
 * Esta función asigna valores a los campos de una estructura de alarma
 * con la información proporcionada.
 *
 * @param id Identificador único de la alarma.
 * @param activa Indicador de activación de la alarma (TRUE o FALSE).
 * @param evento_a_encolar Tipo de evento a encolar asociado a la alarma.
 * @param auxAlarma Valor auxiliar asociado a la alarma.
 * @param retardo_ms Retardo en milisegundos antes de que la alarma se active.
 * @param periodica Indica si la alarma es periódica (TRUE o FALSE).
 * @param tiempo_inicio Marca de tiempo de inicio para alarmas periódicas.
 */
void rellenarAlarma(int id, int activa, enum EVENTO_T evento_a_encolar,
					uint32_t auxAlarma, uint32_t retardo_ms,
					int periodica, uint64_t tiempo_inicio) {
		alarmas[id].activa = activa;
		alarmas[id].evento_a_encolar = evento_a_encolar;
		alarmas[id].auxAlarma = auxAlarma;
		alarmas[id].retardo_ms = retardo_ms;
		alarmas[id].periodica = periodica;
		alarmas[id].tiempo_inicio = tiempo_inicio;
}

//FUNCIONES PUBLICAS

/**
 * Verifica si no hay alarmas activas.
 * Esta función recorre el array de alarmas y verifica si hay alguna activa.
 *
 * @return Devuelve TRUE si no hay alarmas activas, FALSE en caso contrario.
 */
uint8_t no_hay_alarmas() {
		unsigned int i;
		for(i = 0; i < ALARM_MAX; i++){
				if(alarmas[i].activa == TRUE){
						return FALSE;
				}
		}
		return TRUE;
}

/**
 * Inicializa el sistema de alarmas.
 * Esta función configura el sistema de alarmas, establece la función de
 * callback, inicia el temporizador y prepara la cola de alarmas.
 *
 * @param funcion_callback Puntero a la función de callback que se ejecutará al
 * 						   activarse una alarma.
 */
void alarma_inicializar(void(*funcion_callback)()){
		unsigned int i;
		funcion_encolar = funcion_callback;
		temporizador_drv_iniciar();
		temporizador_drv_empezar();
		
		//Inicializar cola de alarmas
		for (i = 0; i < ALARM_MAX; i++){
				alarmas[i].activa = FALSE;
		}
}

/**
 * Activa o reprograma una alarma en el sistema.
 * Esta función activa o reprograma una alarma en función del evento
 * proporcionado, el retardo y los datos auxiliares. También puede cancelar una
 * alarma si el retardo es cero.
 *
 * @param ID_evento Identificador del evento a insertar en la cola al vencer
 * 					la alarma.
 * @param retardo Codifica el retardo en milisegundos y el indicador de alarma
* 				  periódica en el bit más alto.
 *                Si el retardo es cero, la alarma se cancela.
 * @param auxData Campo auxiliar que se encolará en la cola de eventos además
 * 				  del ID_evento.
 */
void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData){
		unsigned int i;
		uint64_t tiempo_actual;
		int periodico = (retardo & 0x80000000) >> 31; //Primer bit
		uint32_t ret = retardo & 0x7FFFFFFF; //Retardo en milisegundos
		
		tiempo_actual = clock_getus();
		
		for(i = 0; (i < ALARM_MAX); i++){
				if (alarmas[i].activa == TRUE &&
					alarmas[i].evento_a_encolar == ID_evento) {
						if (retardo == 0) { // Cancelar alarma
								rellenarAlarma(i, FALSE, ID_VOID, 0, 0, FALSE, 0);
								return;
						}
						else { // Reactivar alarma
								rellenarAlarma(i, TRUE, ID_evento, auxData, ret,
											   periodico, tiempo_actual);
								return;
						}
				}
		}
		for(i = 0; (i < ALARM_MAX); i++){ // Nuevo hueco para alarma
				if (alarmas[i].activa == FALSE) {
						rellenarAlarma(i, TRUE, ID_evento, auxData, ret,
									   periodico, tiempo_actual);
						return;
				}
		}
		funcion_llamar_encolar_alarma(ev_ALARMA_OVERFLOW, 0);
}

/**
 * Verifica si hay que disparar el evento asociado a alguna de las alarmas
 * programadas pendientes.
 * Esta función comprueba el tiempo transcurrido desde el inicio de cada alarma
 * programada y dispara el evento asociado si ha llegado el momento. Las alarmas
 * no periódicas se cancelan tras dispararse, mientras que las periódicas
 * continúan activas de forma indefinida hasta que se eliminan.
 */
void alarma_tratar_eventos(void){
	unsigned int i;
	uint64_t tiempo_actual;
	uint32_t retardo, ret;
	int periodica;
	tiempo_actual = clock_getus();
	
	for(i = 0; i < ALARM_MAX; i++){
			if (alarmas[i].activa == TRUE) {
					if(tiempo_actual - alarmas[i].tiempo_inicio >= alarmas[i].retardo_ms){
							funcion_llamar_encolar_alarma(alarmas[i].evento_a_encolar,
														alarmas[i].auxAlarma);
							if(alarmas[i].periodica == FALSE){ 
									//Si no es periodica -> Desactivar alarma
									alarma_activar(alarmas[i].evento_a_encolar, 0, 0); 
							}
							else { 
									//Si es periodica -> restaurar momento de inicio
									retardo = alarmas[i].retardo_ms;
									periodica = alarmas[i].periodica;
									ret = (retardo & 0x7FFFFFFF) | ((uint32_t)periodica << 31);
									alarma_activar(alarmas[i].evento_a_encolar,
												   ret, alarmas[i].auxAlarma);
							}
					}
			}
	}
}

