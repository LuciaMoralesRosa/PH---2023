/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				planificador.h
*/

#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

// Modulos empleados
#include "temporizador_drv.h"
#include "eventos.h"
#include "gpio_hal.h"
#include "FIFO.h"
#include "io_reserva.h"
#include "power.h"
#include "alarma.h"
#include "swi.h"
#include "WD.h"
#include "linea_serie_drv.h"
#include "botones.h"
#include "hello_world.h"
#include "visualizar.h"
#include "juego.h"
#include "definiciones.h"

// Defincion de la constante USUARIO_AUSENTE para pasar a powerdown
#define USUARIO_AUSENTE 0x00002EE0 //Tiempo en milisegundos (12s)
//#define USUARIO_AUSENTE 0x0000EA60 //Tiempo en milisegundos (60s)

/**
 * Funcion principal del planificador.
 * Esta funcion se encarga de gestionar la planificacion de eventos en el sistema,
 * utilizando una maquina de estados para responder a diferentes eventos.
 */
void planificador(void);

#endif
