/**
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				linea_serie_hal.h
*/

#include "linea_serie_hal.h"

void (*encolar)();
void (*ptr_callback)();
void (*ptr_envio)();

/**
 * Verifica si se ha recibido un carácter en el puerto serie.
 *Esta función utiliza el registro U0IIR para determinar si se ha recibido un carácter
 * en el puerto serie (UART0). La verificación no es bloqueante y se basa en el uso de
 * interrupciones. Retorna un valor entero que indica si hay un carácter disponible para
 * lectura en el registro de datos de recepción (U0RBR).
 *
 * @return Retorna 1 si se ha recibido un carácter, 0 en caso contrario.
 */
unsigned int caracter_recibido(void){
		//Codigo no bloqueante-> usa la interrupcion
		return (U0IIR & 0xe) == 4;
}

/**
 * Rutina de Servicio de Interrupción (ISR) para UART0.
 * Esta función se ejecuta en respuesta a una interrupción de UART0. Marca la interrupción
 * como atendida y verifica si hay un carácter recibido en el registro de datos de recepción
 * (U0RBR). Si hay un carácter, lo lee y lo pasa como argumento a la función de retorno de
 * llamada registrada (apuntada por 'ptr_callback'). En caso contrario, invoca la función de
 * continuación de envío (apuntada por 'ptr_envio').
 */
void uart0_ISR(void) __irq {
		// Marcar como atendida la interrupcion
		VICVectAddr = 0; 
		if (caracter_recibido()) {
				// Si hay un carácter recibido, leerlo del registro de datos de recepción (U0RBR)
				uint32_t caracter_recibido = U0RBR;
				// Invocar la función de retorno de llamada con el carácter recibido como argumento
				(*ptr_callback)(caracter_recibido);
		}
		else{
				// Si no hay un carácter recibido, invocar la función de continuación de envío
				(*ptr_envio)();
		}
}

/**
 * Inicializa el módulo de la línea serie (UART0) y registra funciones de callback.
 * Esta función realiza la configuración inicial del módulo de la línea serie
 * (UART0) y registra las funciones de encolar, callback y envío proporcionadas
 * como parámetros. Configura los pines de la línea serie, los registros de
 * control de línea (U0LCR), el divisor de la línea (U0DLL), y habilita las
 * interrupciones necesarias. Además, registra las funciones de encolar,
 * callback y envío para su uso posterior en las rutinas de interrupción.
 *
 * @param funcion_encolar Puntero a la función de encolar eventos.
 * @param funcion_callback Puntero a la función de callback para manejar
 * 						   caracteres recibidos.
 * @param funcion_envio Puntero a la función de continuación de envío.
 */
void inicializar_linea_serie_hal(void (*funcion_encolar)(),
									   void (*funcion_callback)(),
									   void (*funcion_envio)()){
		// Configuración de pines para UART0
		PINSEL0 |= 0x5;
		// Configuración de la línea de control (U0LCR)
		U0LCR = 0x83;
		// Configuración del divisor de la línea (U0DLL)
		U0DLL = 97;
		// Restaurar la configuración de la línea de control (U0LCR)
		U0LCR = 0X03;
		// Habilitar interrupciones para recibir y transmitir (U0IER)
		U0IER = 3;
		// Configuración de la interrupción en el Vectored Interrupt Controller (VIC)
		VICVectCntl4 = 0x20 | 6;
		VICVectAddr4 = (unsigned long)uart0_ISR;
		// Habilitar la interrupción UART0 en el VIC
		VICIntEnable |= 0x00000040;

		// Registrar las funciones de encolar, callback y envío
		encolar = funcion_encolar;
		ptr_callback = funcion_callback;
		ptr_envio = funcion_envio;
}

/**
 * Envia un carácter a través de la línea serie (UART0).
 * Esta función envía el carácter proporcionado como parámetro a través del registro
 * de transmisión THR (U0THR) de la línea serie (UART0). El carácter se transmitirá
 * tan pronto como sea posible.
 *
 * @param caracter El carácter a enviar a través de la línea serie.
 */
void linea_serie_hal_enviar_caracter(char caracter) {
		U0THR = caracter;
}
