/**
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				juego.h
*/

#ifndef JUEGO_H
#define JUEGO_H

// Defincion de constantes para los comandos validos del juego
//#define TAB 0x00544142 
#define NEW 0x4E455700
#define END 0x454E4400

// Includes
#include <stdint.h>

// Modulos empleados
#include "temporizador_drv.h"
#include "linea_serie_drv.h"
#include "botones.h"
#include "conecta_K_2023.h"
#include "tablero_test.h"
#include "FIFO.h"
#include "eventos.h"

// Definicion de constantes para el tablero
#define MIN_TABLERO 1
#define MAX_TABLERO 7

// Definicion del tamaño de los comandos
#define TAM_BUFFER 3

// Definicion de los posibles estados del juego
enum ESTADO {
	START = 0,
	PLAYING = 1,
	FINISH = 2
};

// Instrucciones y comentarios a mostrar por la linea serie
static char INSTRUCCIONES[200] = "\tINSTRUCCIONES DEL CONECTA-K\n\0";
static char INST1[200] = " - Para colocar una ficha use el comando $#-#!,\n   siendo # la fila y la columna respectivamente.\n\0";
static char INST2[200] = " - Pulse el boton 1 para cancelar la ultima jugada.\n\0";
static char INST3[200] = " - Pulse el boton 2 o escriba el comando $END\n   para rendirse y terminar la partida.\n\0";
static char INST4[200] = " - Ganara la partida el primer jugador que consiga\n   colocar 4 fichas del mismo color seguidas.\n\0";
static char INST5[200] = " - Una vez finalizada una partida,\n   puede iniciar una nueva partida con el comando $NEW!.\n\0";
static char TABLERO_CARGADO[200] = "\n\tSe ha cargado el tablero...\n\tPulse un boton para continuar.\n\n\0";
static char FINJUEGO[200] = "FIN DEL JUEGO\n\0";
static char PREGUNTAR_VOLVER_A_JUGAR[200] = "\nInserte el comando $NEW! o pulse un boton para volver a jugar\n\0";
static char JUEGO_GANADO[200] = "HAS GANADO!\n\0";
static char JUGADA_CANCELADA[200] = "LA JUGADA HA SIDO CANCELADA\n\0";
static char PULSA_CANCELAR[200] = "PULSE EL BOTON 1 PARA CANCELAR\n\0";
static char TURNO_1[200] = "\nEs el turno del JUGADOR 1 (BLANCAS)\n\0";
static char TURNO_2[200] = "\nEs el turno del JUGADOR 2 (NEGRAS)\n\0";
static char GANADOR_1[200] = "HA GANADO EL JUGADOR 1 (BLANCAS)!\n\0";
static char GANADOR_2[200] = "HA GANADO EL JUGADOR 2 (NEGRAS)!\n\0";
static char RENDIDO_1[200] = "EL JUGADOR 1 (BLANCAS) SE HA RENDIDO\n\0";
static char RENDIDO_2[200] = "EL JUGADOR 2 (NEGRAS) SE HA RENDIDO\n\0";
static char NO_PUEDE_CANCELAR_TIEMPO[200] = "NO SE PUEDE CANCELAR LA JUGADA:\n\tSe ha excedido el limite de tiempo.\n\0";
static char NO_PUEDE_CANCELAR_ANTERIOR[200] = "NO SE PUEDE CANCELAR:\n\tNo hay una jugada previa que deshacer\n\0";
static char ERROR_GENERICO[200] = "HA HABIDO UN ERROR\n\0";
static char ERROR_COMANDO_NO_VALIDO[200] = "ERROR: El comando no es valido\n\0";
static char ERROR_FILA[200] = "ERROR: La fila no es correcta\n\0";
static char ERROR_COLUMNA[200] = "ERROR: La columna no es correcta\n\0";
static char ERROR_CELDA_LLENA[200] = "ERROR: Ya hay una ficha en esa posicion\n\0";
static char ERROR_MATRIZ_DISPERSA[200] = "ERROR: No cabe en la matriz dispersa\n\0";

static char MENU_FIN_PARTIDA[200] = "\n\tESTADISTICAS FINAL DE PARTIDA\n\0";
static char CAUSA_END[200] = "\tCausa de finalizacion: Se ha recibido el comando $END!\n\0";
static char CAUSA_BOTON[200] = "\tCausa de finalizacion: Se ha pulsado el boton de reinicio\n\0";
static char CAUSA_VICTORIA[200] = "\tCausa de finalizacion: Se ha ganado la partida\n\0";

static char TIEMPO_PROCESADOR[200] = "\n\tTiempo de ejecucion del procesador: \0";
static char TIEMPO_TOTAL_HAY_LINEA[200] = "\n\tTiempo total de computo de conecta_K_hay_linea: \0";
static char TIEMPO_MEDIA_HAY_LINEA[200] = "\n\tTiempo medio de computo de conecta_K_hay_linea: \0";
static char TIEMPO_TOTAL_PENSAR_JUGADA[200] = "\n\tTiempo total de pensar jugadas: \0";
static char TIEMPO_MEDIA_PENSAR_JUGADA[200] = "\n\tTiempo medio de pensar jugadas: \0";

static char EVENTOS[200] = "\n\n\tEVENTOS: \n\0";

/**
 * Inicializa el juego con la función de callback, el comando de error
 * GPIO y sus bits correspondientes.
 *
 * @param funcion_callback Puntero a la función de callback para encolar eventos.
 * @param comando_error Valor del comando de error GPIO.
 * @param comando_error_bits Bits correspondientes al comando de error GPIO.
 */
void juego_inicializar(void (*funcion_callback)(), unsigned int comando_error,
							 unsigned int comando_error_bits);

/**
 * Trata un evento del juego.
 *
 * @param ID_evento Identificador del evento a tratar (enum EVENTO_T).
 * @param auxData   Datos auxiliares asociados al evento (uint32_t).
 */
void juego_tratar_evento(enum EVENTO_T ID_Evento, uint32_t auxData);

char* itoa(int numero, char* string);


#endif
