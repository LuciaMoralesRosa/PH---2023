/*
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				conecta_K_2023.c
 */

#include "conecta_K_2023.h"

/**
 * Carga el estado a mitad de partida de las primeras 7 filas y columnas en la
 * estructura de datos tablero.
 * Esta función carga un estado predefinido en la estructura de datos tablero
 * para facilitar el test de posibles jugadas. Los valores en la matriz
 * tablero_test indican el contenido de cada casilla
 * (0: vacía, 1: ficha jugador uno, 2: ficha jugador dos).
 *
 * @param t Puntero a la estructura de datos TABLERO.
 */
void conecta_K_test_cargar_tablero(TABLERO *t){
		#include "tablero_test.h"	
		
		for(uint8_t fils = 0; fils < NUM_FILAS; fils++){
				for(uint8_t cols = 0; cols < NUM_COLUMNAS; cols++){
						tablero_insertar_color(t, fils, cols, tablero_test[fils][cols]);
				}
		}
}

/**
 * Carga un estado de tablero proporcionado en la estructura de datos tablero.
 * Esta función carga un estado proporcionado en la estructura de datos tablero.
 * Los valores en la matriz bidimensional 'tablero' indican el contenido de cada
 * casilla.
 *
 * @param t Puntero a la estructura de datos TABLERO.
 * @param tablero Matriz bidimensional que representa el estado del tablero a
 * 				  cargar.
 */
void conecta_K_cargar_tablero(TABLERO *t, uint8_t tablero[7][7])
{	
		for(uint8_t fils = 0; fils < NUM_FILAS; fils++){
				for(uint8_t cols = 0; cols < NUM_COLUMNAS; cols++){
						tablero_insertar_color(t, fils, cols, tablero[fils][cols]);
				}
		}
}

/**
 * Visualiza el contenido de las primeras 7 filas y columnas del tablero en una
 * "pantalla" de memoria.
 * Esta función visualiza en "pantalla" el contenido de las 7 primeras filas y
 * columnas del tablero en juego. La memoria de pantalla debe estar dimensionada
 * como una matriz de 8x8 bytes y verse algo tal que asi:
 * 00 C1 C2 C3 C4 C5 C6 C7
 * F1 00 00 00 00 00 00 00
 * F2 00 00 11 00 00 00 00
 * F3 00 11 22 22 00 00 00
 * F4 00 00 11 00 00 00 00
 * F5 00 00 00 00 00 00 00
 * F6 00 00 00 00 00 00 00
 * F7 00 00 00 00 00 00 00
 *
 * @param t Puntero a la estructura de datos TABLERO.
 * @param pantalla Matriz bidimensional de 8x8 bytes que representa la
 * 				   "pantalla" de memoria.
 */
void conecta_K_visualizar_tablero(TABLERO *t, uint8_t pantalla[8][8]){
		uint8_t fils;
		uint8_t cols;
		
		//Poner 00 en la primera posicion
		pantalla[0][0] = 0x00;
		
		//Numeracion de columnas 
		for(cols = 1; cols <=	NUM_COLUMNAS; cols++){
				pantalla[0][cols] = 0xC0 + cols;
		}
		//Numeracion de filas
		for(fils = 1; fils <= NUM_FILAS; fils++){
				pantalla[fils][0] = 0xF0 + fils;
		}
		
		//Insercion de fichas por pantalla
		for(fils = 0; fils < NUM_FILAS; fils++){
				for(cols = 0; cols < NUM_COLUMNAS; cols++){
						pantalla[fils+1][cols+1] = celda_color(tablero_leer_celda(t, fils, cols)) * 0x11;
				}
		}
}

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
int conecta_K_verificar_K_en_linea(TABLERO *t, uint8_t fila, uint8_t columna,
								   uint8_t color){
		//VERSION ARM-ARM
		uint8_t resultado_arm_arm = conecta_K_hay_linea_arm_arm(t, fila, columna, color);
		return resultado_arm_arm;
}

/**
 * Función principal para el juego Conecta-K.
 * Esta función inicia y controla el juego Conecta-K, permitiendo a los
 * jugadores realizar movimientos y verificando si hay un K en línea después de
 * cada movimiento.
 */
void conecta_K_jugar(void){
		// new, row, column, colour, padding to prevent desalinating to 8 bytes
		static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; //jugada, fila, columna, color, ...
		// 8x8 intentando que este alineada para que se vea bien en memoria
		static uint8_t salida[8][8];
		
		TABLERO cuadricula;
		uint8_t row, column, colour;
		tablero_inicializar(&cuadricula);
		conecta_K_test_cargar_tablero(&cuadricula);
		conecta_K_visualizar_tablero(&cuadricula, salida);

		entrada_inicializar(entrada);
		
		while (1){
			while (entrada_nueva(entrada) == 0){};
			entrada_leer(entrada, &row, &column, &colour);
			//validada la entrada en rango, mirar color valido?
			if(tablero_fila_valida(row) && tablero_columna_valida(column) && tablero_color_valido(colour)){	
				//podriamos no validarla ya que tablero_insertar_valor vuelve a validar
				if (celda_vacia(tablero_leer_celda(&cuadricula, row, column))){
					//tablero_insertar tambien chequea si esta libre esa celda o no...
					if(tablero_insertar_color(&cuadricula, row, column, colour) == EXITO) {
						conecta_K_visualizar_tablero(&cuadricula, salida);
						if(conecta_K_verificar_K_en_linea(&cuadricula, row, column, colour)) {
							while(1); // equivaldria a K_linea encontrada, fin de partida... 
						}
					}
					else {
						while(1); //no cabe en la matriz dispersa, hemos dimensionado mal, error de diseño
					}
				}
				//else: celda no vacia
			}
			//else: fuera de rango fila, columna o color
			entrada_inicializar (entrada);
		}
}

