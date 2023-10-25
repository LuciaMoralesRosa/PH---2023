/*
 * Autor: Morales Rosa, Lucia (816906) y Pavón Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				alarmas.c
*/

#include "alarmas.h"

// El gestor de alarmas generará eventos añadiéndolos a la cola para
// que sean gestionados por el planificador

//Devuelve true si no hay alarmas activas
uint8_t no_hay_alarmas() {
	unsigned int i;
	for(i = 0; i < ALARM_MAX; i++){
			if(alarmas[i].activa == true){
				return true;
			}
		}
	return false;
}

void alarma_inicializar(){
	//Inicializar cola de alarmas
	int i;
	for (i = 0; i < ALARM_MAX; i++){
		alarmas[i].activa = false;
	}
}

//Rellena los campos de la alarma "alarmas[id]" con los valores pasados como argumento
void rellenarAlarma(int id, enum BOOLEANO activa,	enum EVENTO_T evento_a_encolar,
										uint32_t auxAlarma,	uint32_t retardo_ms,	BOOLEANO periodica,
										uint64_t tiempo_inicio){
	alarmas[id].activa = activa;
	alarmas[id].evento_a_encolar = evento_a_encolar;
	alarmas[id].auxAlarma = auxAlarma;
	alarmas[id].retardo_ms = retardo_ms;
	alarmas[id].periodica = periodica;
	alarmas[id].tiempo_inicio = tiempo_inicio;											
}

/*
@param ID_event: determina el evento a insertar en la cola al vencer la alarma
@param retardo: codifica en el bit de más peso si la alarma es periódica (debe repetirse)
								Los restantes bits indican el retardo en milisegundos
@param auxData: campo auxiliar que se encolara en la cola de eventos además del ID_evento.

- Si el retardo es cero se cancelará la alarma.
- Si se reprograma un determinado evento antes de que salte, se reprogramará al nuevo valor (identificador único).
*/
void alarma_activar(enum EVENTO_T ID_evento, uint32_t retardo, uint32_t auxData){
	unsigned int i;
	BOOLEANO periodico;
	BOOLEANO existia_evento = false;
	BOOLEANO hay_hueco = true;
	uint32_t ret;
	
	if(retardo == 0){ //Retardo == 0 -> Cancelar alarma
		for(i = 0; (i < ALARM_MAX); i++){
			if(alarmas[i].evento_a_encolar == ID_evento){
				rellenarAlarma(i, false, ID_evento, 0, 0, false, 0);
			}
		}
	}
	else{ //Retardo != 0
		//gestionar el retardo:
		periodico = (retardo & 0x80000000) >> 31;
		ret = retardo & 0x7FFFFFFF;
		
		for(i = 0; (i < ALARM_MAX) && (!existia_evento); i++){ //Comprobar si ID_evento esta en la cola de alarmas
			if(alarmas[i].evento_a_encolar == ID_evento){ //Reprogramar
				rellenarAlarma(i, alarmas[i].activa, ID_evento, auxData, ret, periodico, timer0_drv_leer());
				existia_evento = true;
			}
		}
		
		if(!existia_evento){ //Crear nueva entrada en la cola de alarmas
			for(i = 0; (i <= ALARM_MAX); i++){ //Busqueda de hueco en la cola alarmas
				if(i == ALARM_MAX){
					hay_hueco = false;
				}
				else if(alarmas[i].activa == false){
					break;
				}
			}

			if(!hay_hueco){	//No hay hueco en alarmas -> Generar evento ALARMA_OVERFLOW
				FIFO_encolar(ALARMA_OVERFLOW, 0);
			}
			else{	//hay hueco en alarmas -> Inicializar la nueva alarma en la cola alarmas
				rellenarAlarma(i, true, ID_evento, auxData, ret, periodico, timer0_drv_leer());
			}
		}
	}
}

/*
Comprobará si hay que disparar el evento asociado a alguna de las alarmas programadas pendientes.
- Las alarmas no periódicas se cancelan tras dispararse.
- Las periódicas continúan activas de forma indefinida hasta que se eliminen.
*/
void alarma_tratar_eventos(void){
	unsigned int i;
	uint64_t tiempo_actual = timer0_drv_leer();
	uint32_t retardo, ret;
	BOOLEANO periodica;
	for(i = 0; i < ALARM_MAX; i++){
		if(tiempo_actual - alarmas[i].tiempo_inicio <= alarmas[i].retardo_ms){
			FIFO_encolar(alarmas[i].evento_a_encolar, alarmas[i].auxAlarma);
			if(alarmas[i].periodica == false){ //Si no es periodica -> Desactivar alarma
				alarma_activar(alarmas[i].evento_a_encolar, 0, 0); 
			}
			else { //Si es periodica -> restaurar momento de inicio
				retardo = alarmas[i].retardo_ms;
				periodica = alarmas[i].periodica;
				ret = (retardo & 0x7FFFFFFF) | ((uint32_t)periodica << 31);
				alarma_activar(alarmas[i].evento_a_encolar, ret, 0);
			}
		}
	}
}

