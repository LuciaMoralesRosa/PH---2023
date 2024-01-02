/**
 * Autor: Morales Rosa, Lucia (816906) y Pavon Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				juego.c
 */

#include "juego.h"

// Funcion para encolar eventos
static void (*funcion_encolar)();

// Variables para manejo de errores en comandos GPIO
unsigned int gpio_comando_error;
unsigned int gpio_comando_error_bits;

// Otras ariables a usar en el modulo
TABLERO tablero_actual;
enum ESTADO estado = START;
static enum EVENTO_T evento;
static uint32_t aux;
uint8_t sePuedeCancelar = 0;
uint8_t jugadorActual;
uint64_t momento_inicio_partida;
uint64_t momento_fin_partida;
int jugada_anterior[2];

// Descomentar siguiente linea para salida en memoria
// static uint8_t salida[8][8];

// Variables para el menu de final de partida
static char causa[200];
static uint64_t t_hay_linea_total;
static uint64_t t_hay_linea_media;
static uint64_t t_pensar_jugada_total;
static uint64_t t_pensar_jugada_media;
static uint64_t t_inicio_pensar_jugada;
static uint64_t t_final_pensar_jugada;
static uint64_t t_aux;
static uint64_t t_aux2;

/**
 * Funcion auxiliar para encolar eventos.
 *
 * @param e Enumeración que representa el tipo de evento.
 * @param a Valor auxiliar de 32 bits asociado al evento.
 */
void funcion_llamar_encolar_juego(enum EVENTO_T e, uint32_t a) {
	evento = e;
	aux = a;
	funcion_encolar(evento, aux);
}

/***** Funciones para el manejo de la pantalla *****/

/**
 * Escribe una cadena en la pantalla del juego por la línea serie.
 *
 * @param cadena Cadena de caracteres que se desea escribir en la pantalla.
 */
void juego_escribir_en_pantalla(char cadena[]) {
	linea_serie_drv_enviar_array(cadena);
}

/**
 * Borra la pantalla del juego.
 *
 * Esta Funcion utiliza la Funcion `linea_serie_drv_limpiar_uart0` para limpiar
 * la pantalla del juego a trav�s de la linea serie UART0.
 */
void juego_borrar_pantalla() {
	linea_serie_drv_limpiar_uart0();
}

/**
 * Convierte el estado actual del tablero a una cadena de caracteres.
 *
 * @param t Puntero al tablero actual.
 * @param cadena Puntero al arreglo de caracteres donde se almacenara la
 * representacion del tablero.
 *
 * @note La cadena resultante incluir� la disposicion de las fichas en el
 * tablero, indicando las posiciones de las fichas blancas ('B' y 'N') y negras
 * ('b' y 'n'), asi como la numeracion de las filas y columnas.
 * Se utiliza la notaci�n 'B' y 'N' para las fichas blancas, 'b' y 'n' para las
 * negras, y ' ' para casillas vac�as. Adem�s, se a�aden etiquetas numeradas
 * para las filas y columnas.
 */
void juego_convertir_tablero_a_array(TABLERO *t, char *cadena) {
	char new_cadena[200];
	uint8_t valor_celda = 0;
	uint8_t iterador = 0;
	uint8_t cols = 0;
	char ficha;

	// Iterar sobre las filas del tablero
	for (uint8_t fils = NUM_FILAS; fils > 0; fils--) {
		// Agregar etiqueta numerada de fila
		new_cadena[iterador] = '0' + fils;
		iterador++;
		new_cadena[iterador] = '|';
		iterador++;

		// Iterar sobre las columnas del tablero
		for (cols = 0; cols < NUM_COLUMNAS; cols++)	{
			valor_celda = 0;
			valor_celda = celda_color(tablero_leer_celda(t, fils - 1, cols)) * 0x11;

			// Determinar el tipo de ficha en la celda
			if (valor_celda == 0x11) {
				if (jugada_anterior[0] == (fils - 1) && jugada_anterior[1] == cols)	{
					ficha = 'b';
				}
				else {
					ficha = 'B';
				}
			}
			else if (valor_celda == 0x22) {
				if (jugada_anterior[0] == (fils - 1) && jugada_anterior[1] == cols)	{
					ficha = 'n';
				}
				else {
					ficha = 'N';
				}
			}
			else {
				ficha = ' ';
			}

			// Agregar ficha y separador
			new_cadena[iterador] = ficha;
			iterador++;
			new_cadena[iterador] = '|';
			iterador++;
		}

		// Agregar salto de línea
		new_cadena[iterador] = '\n';
		iterador++;
	}

	// Agregar línea separadora y etiquetas numeradas de columna
	new_cadena[iterador] = '-';
	iterador++;
	new_cadena[iterador] = '|';
	iterador++;
	for (cols = 0; cols < NUM_COLUMNAS; cols++) {
		new_cadena[iterador] = cols + '1';
		iterador++;
		new_cadena[iterador] = '|';
		iterador++;
	}

	// Agregar saltos de línea y terminador nulo
	new_cadena[iterador] = '\n';
	iterador++;
	new_cadena[iterador] = '\0';
	iterador++;

	//Guardar la cadena resultante en cadena
	strncpy(cadena, new_cadena, iterador);
}

/**
 * Muestra en pantalla el turno del jugador actual.
 */
void juego_mostrar_turno_jugador() {
	switch (jugadorActual)	{
	case 1:
		juego_escribir_en_pantalla(TURNO_1);
		break;
	case 2:
		juego_escribir_en_pantalla(TURNO_2);
		break;
	}
}

/**
 * Muestra en pantalla el jugador ganador.
 */
void juego_mostrar_ganador() {
	switch (jugadorActual) {
	case 1:
		juego_escribir_en_pantalla(GANADOR_1);
		break;
	case 2:
		juego_escribir_en_pantalla(GANADOR_2);
		break;
	}
}

/**
 * Muestra en pantalla el jugador que se ha rendido.
 */
void juego_mostrar_rendido() {
	switch (jugadorActual) {
	case 1:
		juego_escribir_en_pantalla(RENDIDO_1);
		break;
	case 2:
		juego_escribir_en_pantalla(RENDIDO_2);
		break;
	}
}

/**
 * Muestra el tablero en la pantalla.
 * Esta función borra la pantalla, convierte el tablero a una cadena de
 * caracteres y escribe la cadena en la pantalla.
 *
 * @param tablero El tablero que se va a mostrar en la pantalla.
 */
void juego_mostrar_tablero(TABLERO tablero) {
	char cadena[200];
	juego_borrar_pantalla();
	juego_convertir_tablero_a_array(&tablero, cadena);
	juego_escribir_en_pantalla(cadena);
}

/**
 * Muestra las instrucciones del juego en la pantalla.
 * Esta función utiliza la función juego_escribir_en_pantalla para mostrar las
 * instrucciones del juego en la pantalla. 
 */
void juego_mostrar_instrucciones() {
	juego_escribir_en_pantalla(INSTRUCCIONES);
	juego_escribir_en_pantalla(INST1);
	juego_escribir_en_pantalla(INST2);
	juego_escribir_en_pantalla(INST3);
	juego_escribir_en_pantalla(INST4);
	juego_escribir_en_pantalla(INST5);
}

/**
 * Muestra la pantalla de fin de juego y la opción de volver a jugar
 */
void juego_mostrar_final() {
	juego_escribir_en_pantalla(FINJUEGO);
	juego_escribir_en_pantalla(PREGUNTAR_VOLVER_A_JUGAR);
}

/***** Funciones del juego *****/

/**
 * Carga un tablero con los colores proporcionados.
 *
 * @param tab Puntero a la estructura del tablero a cargar.
 * @param tablero Matriz 2D que representa los colores del tablero.
 *                Debe tener dimensiones NUM_FILAS x NUM_COLUMNAS.
 *                Cada elemento representa el color en la posición dada.
 */
void juego_cargar_tablero(TABLERO *tab, uint8_t tablero[7][7]) {
	for (uint8_t fils = 0; fils < NUM_FILAS; fils++) {
		for (uint8_t cols = 0; cols < NUM_COLUMNAS; cols++) {
			tablero_insertar_color(tab, fils, cols, tablero[fils][cols]);
		}
	}
}

/**
 * Cambia el jugador actual en le juego.
 */
void juego_cambiar_jugador() {
	switch (jugadorActual) {
	case 1:
		jugadorActual = 2;
		break;
	case 2:
		jugadorActual = 1;
		break;
	}
}

/**
 * Guarda la jugada actual en las variables globales.
 *
 * @param[in] fila      Fila de la jugada actual.
 * @param[in] columna   Columna de la jugada actual.
 */
void juego_guardar_jugada(uint8_t fila, uint8_t columna) {
	jugada_anterior[0] = fila;
	jugada_anterior[1] = columna;
}

/**
 * Cancela la última jugada realizada, si es posible.
 *
 * Esta función verifica si se puede cancelar la última jugada. En caso
 * afirmativo, cambia al jugador siguiente, borra la celda correspondiente en el
 * tablero, actualiza la jugada anterior, muestra el tablero y el turno del
 * jugador, escribe un mensaje en pantalla indicando que la jugada fue cancelada
 * y actualiza la bandera de cancelación. Si no es posible cancelar, se emite un
 * mensaje correspondiente en pantalla.
 */
void juego_cancelar_jugada() {
	if (sePuedeCancelar) {
		// Cambiar al jugador siguiente
		juego_cambiar_jugador();

		// Borrar la celda correspondiente en el tablero
		tablero_borrar_celda(&tablero_actual, jugada_anterior[0], jugada_anterior[1]);

		// Actualizar la jugada anterior
		jugada_anterior[0] = -1;
		jugada_anterior[1] = -1;

		// Mostrar el tablero actualizado
		juego_mostrar_tablero(tablero_actual);
		// Mostrar el turno del jugador
		juego_mostrar_turno_jugador();
		// Escribir mensaje en pantalla indicando que la jugada fue cancelada
		juego_escribir_en_pantalla(JUGADA_CANCELADA);
		// Actualizar la bandera de cancelación
		sePuedeCancelar = FALSE;
	}
	else {
		// Manejar casos donde no se puede cancelar
		if (jugada_anterior[0] == -1) {
			juego_escribir_en_pantalla(NO_PUEDE_CANCELAR_ANTERIOR);
		}
		else {
			juego_escribir_en_pantalla(NO_PUEDE_CANCELAR_TIEMPO);
		}
	}
}

/**
 * Convierte un evento de tipo enum EVENTO_T a una cadena de texto
 * representativa.
 *
 * @param evento - Enumeración que representa un evento.
 * @return Una cadena de texto que describe el evento.
 */
char *eventoToString(enum EVENTO_T evento) {
	switch (evento)	{
	case ID_VOID:
		return "Total";
	case ev_TRATAR_EVENTO:
		return "ev_TRATAR_EVENTO";
	case ev_BOTON_PULSADO:
		return "ev_BOTON_PULSADO";
	case ev_BOTON_MONITORIZAR:
		return "ev_BOTON_MONITORIZAR";
	case ev_ALARMA_OVERFLOW:
		return "ev_ALARMA_OVERFLOW";
	case ev_LATIDO:
		return "ev_LATIDO";
	case ev_VISUALIZAR_HELLO:
		return "ev_VISUALIZAR_HELLO";
	case ev_USUARIO_AUSENTE:
		return "ev_USUARIO_AUSENTE";
	case ev_RX_SERIE:
		return "ev_RX_SERIE";
	case ev_TX_SERIE:
		return "ev_TX_SERIE";
	case ev_END_GAME:
		return "ev_END_GAME";
	case ev_RST_CANCELACION:
		return "ev_RST_CANCELACION";
	default:
		return "Desconocido";
	}
}

/**
 * Función para mostrar el menú de final de partida con información
 * detallada.
 * Muestra en la pantalla diferentes estadísticas y tiempos relacionados con la
 * partida, así como eventos específicos y sus repeticiones.
 */
void juego_menu_final_partida(void) {
	int t_proc;
	char tiempo[200];

	// Mostrar el menú de final de partida en pantalla
	juego_escribir_en_pantalla(MENU_FIN_PARTIDA);
	// Mostrar la causa de finalización de la partida
	juego_escribir_en_pantalla(causa);

	// Calcular y mostrar el tiempo de procesador utilizado
	t_proc = momento_fin_partida - momento_inicio_partida;
	juego_escribir_en_pantalla(TIEMPO_PROCESADOR);
	sprintf(tiempo, "%d", t_proc);
	juego_escribir_en_pantalla(tiempo);

	// Mostrar el tiempo total empleado en el calculo de hay_linea
	juego_escribir_en_pantalla(TIEMPO_TOTAL_HAY_LINEA);
	sprintf(tiempo, "%d", (int)t_hay_linea_total);
	juego_escribir_en_pantalla(tiempo);

	// Calcular y mostrar el tiempo medio empleado  en el calculo de hay_linea
	juego_escribir_en_pantalla(TIEMPO_MEDIA_HAY_LINEA);
	t_proc = t_hay_linea_total / t_hay_linea_media;
	sprintf(tiempo, "%d", t_proc);
	juego_escribir_en_pantalla(tiempo);

	// Mostrar el tiempo total empleado en pensar las jugadas
	juego_escribir_en_pantalla(TIEMPO_TOTAL_PENSAR_JUGADA);
	sprintf(tiempo, "%d", (int)t_pensar_jugada_total);
	juego_escribir_en_pantalla(tiempo);

	// Calcular y mostrar el tiempo medio empleado en pensar las jugadas
	t_proc = t_pensar_jugada_total / t_pensar_jugada_media;
	juego_escribir_en_pantalla(TIEMPO_MEDIA_PENSAR_JUGADA);
	sprintf(tiempo, "%d", t_proc);
	juego_escribir_en_pantalla(tiempo);

	// Mostrar eventos y repeticiones
	juego_escribir_en_pantalla(EVENTOS);
	for (enum EVENTO_T evento = ID_VOID; evento <= ev_RST_CANCELACION; evento++) {
		sprintf(tiempo, "\tEvento: %s - Repeticiones: %u\n",
				eventoToString(evento), FIFO_estadisticas(evento));
		juego_escribir_en_pantalla(tiempo);
	}
}

/**
 * Finaliza la partida, realiza las acciones necesarias y muestra el menú
 * de final de partida.
 */
void juego_finalizar() {
	gpio_hal_escribir(gpio_comando_error, gpio_comando_error_bits, 0x0);
	estado = FINISH;
	momento_fin_partida = clock_getus();
	juego_menu_final_partida();
	juego_mostrar_final();
}

/**
 * Rinde el juego, mostrando el estado de rendición, cambiando de jugador,
 * mostrando al ganador y finalizando el juego.
 */
void juego_rendirse() {
	juego_mostrar_rendido();
	juego_cambiar_jugador();
	juego_mostrar_ganador();
	juego_finalizar();
}

/**
 * Verifica si un caracter es un número.
 *
 * @param caracter El caracter a verificar.
 * @return Devuelve 1 si el caracter es un número (0-9), y 0 en caso contrario.
 */
int juego_es_numero(uint32_t caracter) {
	return (caracter <= 9);
}

/**
 * Aplica una jugada en el tablero del juego.
 * Esta función inserta el color del jugador actual en la posición especificada
 * del tablero.
 * En caso de que la inserción falle, entra en un bucle infinito indicando un
 * error crítico de diseño.
 *
 * @param fila      La fila en la que se desea insertar el color.
 * @param columna   La columna en la que se desea insertar el color.
 */
void juego_aplicar_jugada(uint8_t fila, uint8_t columna) {
	if (tablero_insertar_color(&tablero_actual, fila, columna, jugadorActual) == ERROR){
		while (1)
			; // No cabe en la matriz dispersa, se ha dimensionado mal.
	}
}

/**
 * Verifica la presencia de K en línea en una posición específica del
 * tablero.
 *
 * @param fila Fila de la posición a verificar.
 * @param columna Columna de la posición a verificar.
 * @return Valor que indica la presencia de K en línea en la posición dada:
 *         - 1 si hay K en línea.
 *         - 0 si no hay K en línea.
 */
uint8_t juego_verificar_K_en_linea(uint8_t fila, uint8_t columna) {
	int valor;
	t_aux = clock_getus();
	valor = conecta_K_verificar_K_en_linea(&tablero_actual, fila, columna,
										   jugadorActual);
	t_aux2 = clock_getus();
	t_hay_linea_total = t_hay_linea_total + (t_aux2 - t_aux);
	t_hay_linea_media = t_hay_linea_media + 1;
	return valor;
}

/**
 * Procesa una jugada en el juego.
 * Esta función toma un dato auxiliar, lo descompone en fila, guion y columna,
 * verifica la validez de la jugada y realiza las acciones correspondientes en
 * el juego, como actualizar el tablero, verificar victoria, cambiar de jugador,
 * entre otras.
 *
 * @param auxData Dato auxiliar que contiene la información de la jugada.
 * Bits 24-17: fila, bits 16-9: guion, bits 8-1: columna.
 */
void juego_procesar_jugada(uint32_t auxData) {
	// Descomposición del dato auxiliar en fila, guion y columna
	uint8_t fila = (((auxData >> 24) & 0xFF) - '0') - 1;
	uint8_t guion = (auxData >> 16) & 0xFF;
	uint8_t columna = (((auxData >> 8) & 0xFF) - '0') - 1;

	// Verificación de la validez de la jugada
	if ((juego_es_numero(fila) == FALSE) || (juego_es_numero(columna) == FALSE)
		|| (guion != '-')) {
		gpio_hal_escribir(gpio_comando_error, gpio_comando_error_bits, 0x1);
		juego_escribir_en_pantalla(ERROR_COMANDO_NO_VALIDO);
	}
	else if (tablero_fila_valida(fila) == FALSE) {
		gpio_hal_escribir(gpio_comando_error, gpio_comando_error_bits, 0x1);
		juego_escribir_en_pantalla(ERROR_FILA);
	}
	else if (tablero_fila_valida(columna) == FALSE) {
		gpio_hal_escribir(gpio_comando_error, gpio_comando_error_bits, 0x1);
		juego_escribir_en_pantalla(ERROR_COLUMNA);
	}
	else if (celda_vacia(tablero_leer_celda(&tablero_actual, fila, columna))
			 == FALSE) {
		juego_escribir_en_pantalla(ERROR_CELDA_LLENA);
	}
	else {
		// Acciones correspondientes a una jugada válida
		gpio_hal_escribir(gpio_comando_error, gpio_comando_error_bits, 0x0);
		alarma_activar(ev_RST_CANCELACION, 0x00000BB8, 0);
		juego_guardar_jugada(fila, columna);
		juego_aplicar_jugada(fila, columna);

		// Verificación de victoria
		if (juego_verificar_K_en_linea(fila, columna)) {
			juego_mostrar_tablero(tablero_actual);
			juego_mostrar_ganador();
			strcpy(causa, CAUSA_VICTORIA);
			juego_finalizar();
		}
		else {
			// Acciones para continuar el juego
			sePuedeCancelar = TRUE;
			juego_cambiar_jugador();
			t_inicio_pensar_jugada = clock_getus();
			juego_mostrar_tablero(tablero_actual);
			juego_mostrar_turno_jugador();
			juego_escribir_en_pantalla(PULSA_CANCELAR);
		}
	}
}

/**
 * Prepara el juego para una nueva partida.
 *
 * @param tablero_a_cargar Arreglo bidimensional (7x7) que representa el tablero
 * 						   a cargar. Contiene la configuración inicial del juego.
 */
void juego_preparar_partida(uint8_t tablero_a_cargar[7][7]) {
	// Inicializa el tablero actual
	tablero_inicializar(&tablero_actual);
	// Carga el tablero con la configuración proporcionada
	juego_cargar_tablero(&tablero_actual, tablero_a_cargar);

	// Establece la jugada anterior como no válida
	jugada_anterior[0] = -1;
	jugada_anterior[1] = -1;

	// Reinicia los tiempos de medición
	t_hay_linea_total = 0;
	t_hay_linea_media = 0;
	t_pensar_jugada_media = 0;
	t_pensar_jugada_total = 0;
	t_inicio_pensar_jugada = 0;
	t_final_pensar_jugada = 0;

	sePuedeCancelar = FALSE;
	estado = START;
	juego_escribir_en_pantalla(TABLERO_CARGADO);
}

/**
 * Inicializa el juego para realizar pruebas con el tablero de ConectaK.
 * Esta función borra la pantalla, muestra las instrucciones del juego,
 * establece el jugador actual como 1 y prepara la partida con el tablero
 * proporcionado en "tablero_test.h".
 */
void juego_init_test_conectaK() {
	#include "tablero_test.h"
	juego_borrar_pantalla();
	juego_mostrar_instrucciones();
	jugadorActual = 1;
	juego_preparar_partida(tablero_test);
}

/**
 * Inicializa un juego vacío de Conecta K.
 * Esta función realiza la inicialización de un juego vacío de Conecta K,
 * estableciendo el jugador actual, borrando la pantalla y preparando la partida
 * con un tablero vacío definido en "tablero_test.h".
 */
void juego_init_vacio_conectaK() {
	#include "tablero_test.h"
	juego_borrar_pantalla();
	jugadorActual = 2;
	juego_preparar_partida(tablero_test_vacio);
}

/**
 * Inicia una nueva partida de juego.
 * Esta función muestra el tablero actual, el turno del jugador y registra
 * el momento de inicio de la partida.
 *
 * @param tablero_actual Estructura que representa el tablero actual del juego.
 */
void juego_comenzar_partida() {
	juego_mostrar_tablero(tablero_actual);
	juego_mostrar_turno_jugador();
	momento_inicio_partida = clock_getus();
	estado = PLAYING;
}

/**
 * Trata un comando en el juego.
 * Esta función se encarga de procesar comandos específicos del juego
 * y realizar acciones correspondientes según el estado actual del juego.
 *
 * @param auxData Un valor de entrada que representa el comando a tratar.
 */
void juego_tratar_comando(uint32_t auxData) {
	switch (auxData) {
	case NEW:
		juego_init_vacio_conectaK();
		break;
	case END:
		switch (estado) {
		case START:
			break;
		case FINISH:
			break;
		default:
			strcpy(causa, CAUSA_END);
			juego_rendirse();
			break;
		}
		break;
	default:
		switch (estado) {
		case START:
			break;
		case FINISH:
			break;
		default:
			juego_procesar_jugada(auxData);
			break;
		}
		break;
	}
}

/**
 * Trata el evento de pulsación de botón en el juego.
 * Esta función gestiona la pulsación de botones durante diferentes estados del
 * juego.
 *
 * @param auxData Valor auxiliar de 32 bits que puede contener información
 * 				  adicional.
 */
void juego_tratar_boton(uint32_t auxData) {
	switch (estado) {
	case START:
		juego_comenzar_partida();
		break;
	case PLAYING:
		switch (auxData) {
		case EINT1:
			juego_cancelar_jugada();
			break;
		case EINT2:
			strcpy(causa, CAUSA_BOTON);
			juego_rendirse();
			break;
		}
		break;
	case FINISH:
		juego_init_vacio_conectaK();
	}
}

/**
 * Trata un evento del juego.
 *
 * @param ID_evento Identificador del evento a tratar (enum EVENTO_T).
 * @param auxData   Datos auxiliares asociados al evento (uint32_t).
 */
void juego_tratar_evento(enum EVENTO_T ID_evento, uint32_t auxData) {
	uint32_t tiempo_pensar_jugada;
	switch (ID_evento) {
	case ev_BOTON_PULSADO:
		juego_tratar_boton(auxData);
		break;
	case ev_RST_CANCELACION:
		sePuedeCancelar = FALSE;
		break;
	case ev_RX_SERIE:
		t_final_pensar_jugada = clock_getus();
		tiempo_pensar_jugada = t_final_pensar_jugada - t_inicio_pensar_jugada;
		t_pensar_jugada_total = t_pensar_jugada_total + tiempo_pensar_jugada;
		t_pensar_jugada_media++;
		juego_tratar_comando(auxData);
		break;
	default:
		while (1)
			; // error
	}
}

/**
 * Inicializa el juego con la función de callback, el comando de error
 * GPIO y sus bits correspondientes.
 *
 * @param funcion_callback Puntero a la función de callback para encolar eventos.
 * @param comando_error Valor del comando de error GPIO.
 * @param comando_error_bits Bits correspondientes al comando de error GPIO.
 */
void juego_inicializar(void (*funcion_callback)(), unsigned int comando_error,
					   unsigned int comando_error_bits) {
	funcion_encolar = funcion_callback;
	gpio_comando_error = comando_error;
	gpio_comando_error_bits = comando_error_bits;
	juego_init_test_conectaK();
}
