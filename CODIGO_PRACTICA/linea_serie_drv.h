/**
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				linea_serie_drv.h
*/

#ifndef LINEA_SERIE_DRV
#define LINEA_SERIE_DRV

#define NUM_PETICIONES 40

#include <stdint.h>
#include "linea_serie_hal.h"
#include "definiciones.h"
#include "eventos.h"
#include "alarma.h"

/**
 * Inicializa el controlador de la linea serie.
 * Esta funcion configura y prepara el controlador de la linea serie para su uso.
 *
 * @param serie_error Numero de error asociado a la linea serie.
 * @param funcion_encolar_evento Puntero a la funcion para encolar eventos.
 * @param usuario_ausente Tiempo de ausencia del usuario.
 * @param ausente Evento asociado a la ausencia del usuario.
 */
void linea_serie_drv_inicializar(unsigned int serie_error,
                                 void (*funcion_encolar_evento)(),
                                 int usuario_ausente, enum EVENTO_T ausente);

/**
 * Envia un array de caracteres a travos del driver de linea serie.
 *
 * @param cadena Array de caracteres a enviar.
 */
void linea_serie_drv_enviar_array(char cadena[]);

/**
 * Continua el envio de datos a traves de la linea serie.
 * Esta funcion continua el envio de datos a traves de la linea serie,
 * realizando las operaciones necesarias para procesar el buffer interno y
 * enviar los datos de manera secuencial.
 */
void linea_serie_drv_continuar_envio(void);

/**
 * Limpia la pantalla de la terminal a traves de UART0.
 */
void linea_serie_drv_limpiar_uart0(void);

#endif

