/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				fifo.c
*/
#include "fifo.h"

#define NUM_EVENTOS 4

static struct Evento cola[SIZE];
int ini = -1;
int fin = -1;


static uint32_t estadisticas[NUM_EVENTOS];

GPIO_HAL_PIN_T led_overflow;

// Devuelve true si la cola esta vacia
uint8_t esta_vacia() {
    return (ini == -1) && (fin == -1);
}

// Función para verificar si la cola está llena
uint8_t esta_llena() {
    return ((fin + 1) % SIZE) == ini;
}

void FIFO_inicializar(GPIO_HAL_PIN_T pin_overflow) {
	unsigned int i;
	led_overflow = pin_overflow;
	ini = -1;
	fin = -1;
	
	for(i = 0; i < NUM_EVENTOS; i++){
		estadisticas[i] = 0;
	}
}

void FIFO_encolar(enum EVENTO_T ID_evento, uint32_t auxData) {
	struct Evento evento;
	if (esta_llena()) {
    gpio_hal_escribir(led_overflow, GPIO_OVERFLOW_BITS, 0x1); // Encender el LED
		while(1); // Error en la cola, bucle infinito
	}
	else if (esta_vacia()) {
		ini = 0;
		fin = 0;
		evento.Evento_ID = ID_evento;
		evento.auxData = auxData;
		cola[fin] = evento;
  }
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

// Si hay eventos sin procesar, devuelve un valor distinto de cero y el evento
// más antiguo sin procesar por referencia. Cero indicará que la cola está
// vacía y no se ha devuelto ningún evento.
uint8_t FIFO_extraer(enum EVENTO_T *ID_evento, uint32_t* auxData) {
	if (esta_vacia()) {
		return 0;
	}
	*ID_evento = cola[ini].Evento_ID;
	*auxData = cola[ini].auxData;
	if (ini == fin) {
		ini = fin = -1;
	}
	else {
		ini = (ini + 1) % SIZE;
	}
	return 1;
}

// Dado un identificador de evento nos devuelve el número total de veces que ese
// evento se ha encolado. El evento VOID nos devolverá el total de
// eventos encolados desde el inicio.
uint32_t FIFO_estadisticas(enum EVENTO_T ID_evento) {
	return estadisticas[ID_evento];
}

