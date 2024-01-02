/**
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				linea_serie_drv.c
*/

#include "linea_serie_drv.h"

//Variables globales
static unsigned int GPIO_SERIE_ERROR;
static void (*funcion_encolar)();
static enum EVENTO_T evento;
static uint32_t aux;

static enum EVENTO_T usuario_ausente_evento;
static int usuario_ausente_tiempo;

static int led_encendido;
static int comando_valido;
static int comando_iniciado;
static uint32_t longitud_comando;

static uint32_t comando[3];
static char buffer_interno[200];
static uint32_t iterador;

static char mensajes_pendientes[NUM_PETICIONES][200];
static uint8_t numero_pendientes;
static int enviando_mensaje;


/**
 * Llama a la funcion de encolar eventos.
 */
void funcion_llamar_encolar_linea_serie(){
		funcion_encolar(evento, aux);
}

/**
 * Enciende el led de overflow de comando
 */
void linea_serie_hal_encender_led(void){
		led_encendido = TRUE;
		gpio_hal_escribir(GPIO_SERIE_ERROR, 1, 0x1);
}

/**
 * Enciende un LED a traves de la interfaz de linea serie.
 * Esta funcion enciende el LED especificado a traves de la interfaz de linea serie.
 * Tambien actualiza el estado del LED encendido.
 */
void linea_serie_hal_apagar_led(void){
		led_encendido = FALSE;
		gpio_hal_escribir(GPIO_SERIE_ERROR, 1, 0x0);
}

/**
 * Envia un comando mediante la serie.
 *
 * Esta funcion prepara un comando para ser enviado a traves de la comunicacion serie.
 */
void enviar_comando(void){
		aux = (comando[0] << 24 | comando[1] << 16 | comando[2] << 8);
		evento = ev_RX_SERIE;
		funcion_llamar_encolar_linea_serie();
}

/**
 * Confirma el envio de datos por serie.
 */
void confirmar_envio(void){
		aux = 0;
		evento = ev_TX_SERIE;
		funcion_llamar_encolar_linea_serie();
}

/**
 * Detecta un caracter y realiza acciones segun el valor recibido.
 * Esta funcion activa una alarma y procesa un caracter recibido, tomando
 * decisiones segun el valor del caracter. Puede iniciar o finalizar un
 * comando, encender o apagar un LED, y enviar un comando si se cumplen
 * ciertas condiciones.
 *
 * @param caracter Valor del caracter a procesar.
 */
void caracter_detectado(uint32_t caracter){
		alarma_activar(usuario_ausente_evento, usuario_ausente_tiempo, 0);
		switch(caracter){
				case '$':
						comando_iniciado = TRUE;
						longitud_comando = 0;
						comando_valido = TRUE;
						if(led_encendido){
								linea_serie_hal_apagar_led();
						}
						//Resetear el comando
						comando[0] = 0;
						comando[1] = 0;
						comando[2] = 0;
						break;
				case '!':
						if(comando_iniciado && comando_valido){
								comando_iniciado = FALSE;
								comando_valido = FALSE;
								enviar_comando();
						}
						break;
				default:
						if(!led_encendido){
								if(longitud_comando == 3){
										linea_serie_hal_encender_led();
										comando_valido = FALSE;
								}
								else{
										comando[longitud_comando] = caracter;
										longitud_comando++;
										if(longitud_comando == 3){
												comando_valido = TRUE;
										}
								}
						}
						break;
		}
}

/**
 * Inicializa el controlador de la linea serie.
 * Esta funcion configura y prepara el controlador de la linea serie para su uso.
 *
 * @param serie_error Numero de error asociado a la linea serie.
 * @param funcion_encolar_evento Puntero a la funcion para encolar eventos.
 * @param usuario_ausente Tiempo de ausencia del usuario.
 * @param ausente Evento asociado a la ausencia del usuario.
 */
void linea_serie_drv_inicializar(unsigned int serie_error, void (*funcion_encolar_evento)(),
																int usuario_ausente, enum EVENTO_T ausente){
		funcion_encolar = funcion_encolar_evento;
		usuario_ausente_evento = ausente;
		usuario_ausente_tiempo = usuario_ausente;
		GPIO_SERIE_ERROR = serie_error;
		inicializar_linea_serie_hal(funcion_llamar_encolar_linea_serie, caracter_detectado, linea_serie_drv_continuar_envio);
		led_encendido = FALSE;
		comando_valido = FALSE;
		comando_iniciado = FALSE;
		longitud_comando = 0;	
	
		enviando_mensaje = FALSE;
		numero_pendientes = 0;
}

/**
 * Inicia el envio de un nuevo mensaje a traves de la linea serie.
 * Esta funcion toma el primer mensaje pendiente en la cola de mensajes,
 * copia su contenido al buffer interno y comienza a enviar sus caracteres
 * uno por uno mediante la funcion `linea_serie_hal_enviar_caracter`. La funcion
 * se encarga de gestionar el estado de envio y actualizar la cola de mensajes.
 */
void linea_serie_drv_nuevo_envio(void){
		if(enviando_mensaje == FALSE){
				iterador = 0;
				memset(buffer_interno, 0, 200);
				strcpy(buffer_interno, mensajes_pendientes[0]);
				for(uint8_t i = 0; i < numero_pendientes; i++){
						strcpy(mensajes_pendientes[i], mensajes_pendientes[i+1]);
				}
				numero_pendientes--;
				linea_serie_hal_enviar_caracter(buffer_interno[iterador]);
				enviando_mensaje = TRUE;
		}
}

/**
 * Envia un array de caracteres a travos del driver de linea serie.
 *
 * @param cadena Array de caracteres a enviar.
 */
void linea_serie_drv_enviar_array(char cadena[]){
		// Verifica si hay espacio disponible para mas peticiones
		if(numero_pendientes < NUM_PETICIONES){
				numero_pendientes++;
				strcpy(mensajes_pendientes[numero_pendientes-1], cadena);
		}
		linea_serie_drv_nuevo_envio();
}

/**
 * Continua el envio de datos a traves de la linea serie.
 * Esta funcion continua el envio de datos a traves de la linea serie,
 * realizando las operaciones necesarias para procesar el buffer interno y
 * enviar los datos de manera secuencial.
 */
void linea_serie_drv_continuar_envio(void){
		alarma_activar(usuario_ausente_evento, usuario_ausente_tiempo, 0);
		iterador++;
		if (buffer_interno[iterador] == '\0') {
      confirmar_envio();
			enviando_mensaje = FALSE;
			if(numero_pendientes>0){
				linea_serie_drv_nuevo_envio();
			}
    }
    else {
        linea_serie_hal_enviar_caracter(buffer_interno[iterador]);
    }
}

/** 
 * Limpia la pantalla de la terminal a traves de UART0.
 */
void linea_serie_drv_limpiar_uart0(void){
		linea_serie_drv_enviar_array("\x1b[1;1f\x1b[J");
}

