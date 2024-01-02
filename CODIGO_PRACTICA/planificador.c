/**
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				planificador.c
*/

#include "planificador.h"

//Definicion de variables
static enum EVENTO_T evento;
static uint32_t aux;


/**
 * Encola un evento junto con un valor auxiliar en la FIFO.
 * Esta funcion asigna el evento y el valor auxiliar a las variables globales
 * 'evento' y 'aux', respectivamente, y luego encola estos valores en la FIFO.
 *
 * @param id_evento Enumeracion que representa el tipo de evento a encolar.
 * @param auxiliar Valor auxiliar de 32 bits a asociar con el evento.
 */
void funcion_llamada_encolar(enum EVENTO_T id_evento, uint32_t auxiliar){
		evento=id_evento;
		aux=auxiliar;
		FIFO_encolar(evento, aux);
}

/**
 * Gestiona la alarma de overflow.
 * Esta funcion enciende el LED asociado al overflow y detiene el temporizador.
 */
void gestionar_alarma_overflow(void){
		gpio_hal_escribir(GPIO_OVERFLOW, GPIO_OVERFLOW_BITS, 0x1);
		temporizador_drv_parar();	
}


/**
 * Funcion principal del planificador.
 * Esta funcion se encarga de gestionar la planificacion de eventos en el sistema,
 * utilizando una maquina de estados para responder a diferentes eventos.
 */
void planificador(void){
		//Declaracion de variables locales
		enum EVENTO_T id_eventos;
		uint32_t aux_data;
		int hay_eventos;

		//Inicializacion de modulos del sistema
		FIFO_inicializar(GPIO_OVERFLOW);
		gpio_hal_iniciar();
		temporizador_drv_iniciar();
		linea_serie_drv_inicializar(GPIO_SERIE_ERROR, funcion_llamada_encolar, USUARIO_AUSENTE, ev_USUARIO_AUSENTE);
		alarma_inicializar(funcion_llamada_encolar);
		boton_init(funcion_llamada_encolar, USUARIO_AUSENTE, ev_BOTON_PULSADO, ev_BOTON_MONITORIZAR, ev_USUARIO_AUSENTE);
		hello_world_inicializar(funcion_llamada_encolar);
		visualizar_inicializar();
		temporizador_drv_reloj(1, funcion_llamada_encolar, ev_TRATAR_EVENTO);
		alarma_activar(ev_USUARIO_AUSENTE, USUARIO_AUSENTE, 0);
		juego_inicializar(funcion_llamada_encolar, GPIO_COMANDO_ERROR, GPIO_COMANDO_ERROR_BITS);
		
		//Maquina de estados
		while(1){
				//Gestion de eventos
				hay_eventos = FIFO_extraer(&id_eventos, &aux_data);
				if(hay_eventos == TRUE){
						//Procesar eventos segun la maquina de estados
						switch(id_eventos){
								case ID_VOID:
										break;
								case ev_TRATAR_EVENTO:
										alarma_tratar_eventos();
										break;
								case ev_BOTON_MONITORIZAR:
										boton_gestion(aux_data);
										break;
								case ev_BOTON_PULSADO:
										juego_tratar_evento(id_eventos, aux_data);
										break;
								case ev_ALARMA_OVERFLOW:
										gestionar_alarma_overflow();
										break;
								case ev_LATIDO:
										hello_world_tratar_evento();
										break;
								case ev_VISUALIZAR_HELLO:
										visualizar_hello();
										break;
								case ev_USUARIO_AUSENTE:
										power_hal_deep_sleep();
										break;
								case ev_RX_SERIE:
										juego_tratar_evento(id_eventos, aux_data);
										break;
								case ev_TX_SERIE:
										break;
								case ev_END_GAME:
										WD_hal_test();
										break;
								case ev_RST_CANCELACION:
										juego_tratar_evento(id_eventos, aux_data);
										break;
								default:
										while(1); //Error: evento no reconocido
						}
				}
				else{ 
						// La cola de eventos esta vacia y no hay eventos que tratar
						power_hal_wait();
				}			
		}
}
