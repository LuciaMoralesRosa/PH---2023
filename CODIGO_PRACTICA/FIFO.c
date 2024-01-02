/*
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				fifo.c
*/
#include "fifo.h"

// Declaración de una cola de eventos
static struct Evento cola[SIZE];

// Variables para indicar el inicio y el final de la cola
int ini = -1;
int fin = -1;

// Arreglo para almacenar las estadisticas relacionadas con los eventos
static uint32_t estadisticas[NUM_EVENTOS];

// Representacion del pin GPIO asociado al LED de overflow
GPIO_HAL_PIN_T led_overflow;

/**
 * Inicializa la cola FIFO y las estadísticas de eventos.
 * Esta función inicializa la cola FIFO, el LED de overflow y las estadísticas
 * de eventos.
 *
 * @param pin_overflow Pin GPIO asociado al LED de overflow.
 */
void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
		unsigned int i;
		led_overflow = pin_overflow;
		ini = -1;
		fin = -1;
		for(i = 0; i < NUM_EVENTOS; i++){
				estadisticas[i] = 0;
		}
}

/**
 * Encola un evento en la cola FIFO.
 * Esta función añade un nuevo evento a la cola FIFO, actualiza los índices de
 * la cola y las estadísticas.
 *
 * @param ID_evento Identificador del evento a encolar.
 * @param auxData Datos auxiliares asociados al evento.
 */
void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData) {
		struct Evento evento;

		// Verificar si la cola esta vacia
		if (((fin + 1) % SIZE) == ini) {
				// Encender el LED de overflow y entrar en un bucle infinito
				gpio_hal_escribir(led_overflow, 1, 0x1);
				while (1); // Error en la cola, bucle infinito
		}
		// Verificar si la cola está vacía
		else if ((ini == -1) && (fin == -1)) {
				ini = 0;
				fin = 0;
				evento.Evento_ID = ID_evento;
				evento.auxData = auxData;
				cola[fin] = evento;
		}
		// Cola no vacia
		else {
				fin = (fin + 1) % SIZE;
				evento.Evento_ID = ID_evento;
				evento.auxData = auxData;
				cola[fin] = evento;
		}
		//Actualizar estadisticas
		estadisticas[0]++;
		estadisticas[ID_evento]++;
}

/**
 * Extrae el evento más antiguo sin procesar de la cola FIFO.
 * Esta función devuelve un valor distinto de cero y el evento más antiguo sin
 * procesar por referencia si hay eventos en la cola. Cero indica que la cola
 * está vacía y no se ha devuelto ningún evento.
 *
 * @param ID_evento Puntero para almacenar el identificador del evento extraído.
 * @param auxData Puntero para almacenar los datos auxiliares del evento extraído.
 * @return Valor distinto de cero si hay eventos sin procesar, cero si la cola está vacía.
 */
uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t* auxData) {
		// Verificar si la cola esta vacia
		if ((ini == -1) && (fin == -1)) {
				return 0;
		}

		// Extraer el evento mas antiguo sin procesar
		*ID_evento = cola[ini].Evento_ID;
		*auxData = cola[ini].auxData;

		// Verificar si la cola tiene un solo elemento
		if (ini == fin) {
				// Reiniciar los indices si la cola tiene un solo elemento
				ini = fin = -1;
		}
		else {
				// Actualizar el indice de inicio
				ini = (ini + 1) % SIZE;
		}
		return 1;
}

/**
 * Obtiene las estadísticas de eventos de la cola FIFO.
 * Esta función devuelve las estadísticas de eventos asociadas a un evento
 * específico o al total de eventos, según el identificador de evento proporcionado.
 *
 * @param ID_evento Identificador del evento del cual se desean obtener las estadísticas.
 * @return Valor de las estadísticas asociadas al evento específico o al total de eventos.
 */
uint32_t FIFO_estadisticas(enum EVENTO_T ID_evento) {
		uint32_t ret = 0;

		// Verificar si se piden las estadisticas totales
		if(ID_evento == ID_VOID){
				ret = estadisticas[0];
		}
		// Estadisticas para un evento especifico
		else{
				ret = estadisticas[ID_evento];
		}
		return ret;
}
