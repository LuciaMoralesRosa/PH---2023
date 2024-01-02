#ifndef CONECTA_K_H_2023
#define CONECTA_K_H_2023

//Includes
#include <inttypes.h>
#include <stdint.h>

//Modulos empleados
#include "celda.h"
#include "tablero.h"
#include "entrada.h"

/**
 * Función principal para el juego Conecta-K.
 * Esta función inicia y controla el juego Conecta-K, permitiendo a los
 * jugadores realizar movimientos y verificando si hay un K en línea después de
 * cada movimiento.
 */
void conecta_K_jugar(void);

// devuelve la longitud de la línea más larga
uint8_t conecta_K_buscar_alineamiento_c(TABLERO *t, uint8_t fila,
	uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna);

// devuelve true si encuentra una línea de longitud mayor o igual a 4
uint8_t conecta_K_hay_linea_c_c(TABLERO *t, uint8_t fila, uint8_t
	columna, uint8_t color);

// funciones a implementar en ARM

// devuelve la longitud de la línea más larga en un determinado sentido
uint8_t conecta_K_buscar_alineamiento_arm(TABLERO *t, uint8_t
	fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna);

// devuelve true si encuentra una línea de longitud mayor o igual a K
uint8_t conecta_K_hay_linea_arm_c(TABLERO *t, uint8_t fila, uint8_t
	columna, uint8_t color);

// devuelve true si encuentra una línea de longitud mayor o igual a K
uint8_t conecta_K_hay_linea_arm_arm(TABLERO *t, uint8_t fila, uint8_t
	columna, uint8_t color);



/**
 * Verifica si hay un K en línea en una posición específica del tablero.
 * Esta función verifica si hay un K en línea en la posición específica del
 * tablero, utilizando la implementación ARM-ARM.
 *
 * @param t Puntero a la estructura de datos TABLERO.
 * @param fila Fila de la posición a verificar.
 * @param columna Columna de la posición a verificar.
 * @param color Color del K a verificar (1 o 2).
 * @return 1 si hay un K en línea, 0 en caso contrario.
 */
int conecta_K_verificar_K_en_linea(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color);

void conecta_K_cargar_tablero(TABLERO *t, uint8_t tablero[7][7]);

#endif /* CONECTA_K_H_2023 */
