;-------------------------------------- VERSION EN C -----------------------------------------

;uint8_t
;conecta_K_hay_linea_c_c(TABLERO *t, uint8_t fila, uint8_t columna, uint8_t color)
;{
;	enum { N_DELTAS = 4};
;   int8_t deltas_fila[N_DELTAS] = {0, -1, -1, 1};
;   int8_t deltas_columna[N_DELTAS] = {-1, 0, -1, -1};
;   unsigned int i = 0;
;   uint8_t linea = FALSE;
;   uint8_t long_linea = 0;

;   // buscar linea en fila, columna y 2 diagonales
;   for(i=0; (i < N_DELTAS) && (linea == FALSE); ++i) {
;       // buscar sentido
;       long_linea = conecta_K_buscar_alineamiento_c(t, fila, columna, color, deltas_fila[i], deltas_columna[i]);
;       linea = long_linea >= K_SIZE;
;       if (linea) {
;         continue;
;       }
;       // buscar sentido inverso
;       long_linea += conecta_K_buscar_alineamiento_c(t, fila-deltas_fila[i],
;	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);
;       linea = long_linea >= K_SIZE;
;   }
;   return linea;
;}

;----------------------------- IMPLEMENTACION ARM -----------------------------------

		AREA datos, DATA
K_SIZE			EQU		0x00000004		; enum { N_DELTAS = 4}

deltas_fila		EQU		0x00FFFF01	; int8_t deltas_fila[N_DELTAS]    = { 0, -1, -1,  1};
deltas_columna	EQU		0xFF00FFFF	; int8_t deltas_columna[N_DELTAS] = {-1,  0, -1, -1};

		AREA codigo, CODE
		;IMPORT conecta_K_buscar_alineamiento_arm
		IMPORT tablero_buscar_color
		EXPORT conecta_K_hay_linea_arm_arm
		PRESERVE8 {TRUE}

conecta_K_hay_linea_arm_arm
	stmdb	r13!,{r11,r14}
	mov		r11,r13
	stmdb	r13!,{r4-r10,r12}
	
	mov		r7, r0	; r7 = tablero
	mov		r8, r1	; r8 = fila
	mov		r9, r2	; r9 = columna
	mov		r10, r3	; r10 = color

	eor		r6, r6, r6				; r6: i = 0

	;buscar linea en fila, columna y 2 diagonales
inicioBucle
	; Detectar nueva linea
	LDR		r12, =K_SIZE		; long linea
	
	;Delta
	LDR		r4, =deltas_fila
	mov		r4, r4, LSL r6
	mov		r4, r4, ASR #24

	LDR		r5, =deltas_columna
	mov		r5, r5, LSL r6
	mov		r5, r5, ASR #24
	
buscar_alineamiento
	stmdb	r13!,{r1-r2}
	;Comprobar si la celda es valida y del mismo color
	BL		tablero_buscar_color	; Saltar a funcion, devuelve el return a traves de r0
	
	ldmia	r13!,{r1,r2}
	cmp		r0, #0
	bne		finalFunc
	
	sub		r12, r12, #1
	cmp 	r12, #0
	movle 	r0, #1
	ble 	salirFor
	
	;encontrada, entonces avanzar índices
	add		r1, r1, r4				; r1 = nueva_fila = fila + delta_fila
	add		r2, r2, r5				; r2 = nueva_columna = columna + delta_columna
	
	;incrementar longitud y visitar celda vecina
	mov		r0, r7
	mov		r3, r10
	b 		buscar_alineamiento	; Saltar a funcion
	
	;Terminar funcion
finalFunc

	;Buscar sentido inverso
	mov		r0, r7
	mov		r3, r10
	rsb 	r4, r4, #0			; -deltas_fila[i]
	rsb		r5, r5, #0			; -deltas_columna[i]
	add 	r1, r8, r4			; fila = fila - deltas_fila[i]
	add 	r2, r9, r5			; columna = columna - deltas_columna[i]

buscar_alineamiento2
	stmdb	r13!,{r1-r2}
	;Comprobar si la celda es valida y del mismo color
	BL		tablero_buscar_color	; Saltar a funcion, devuelve el return a traves de r0
	
	ldmia	r13!,{r1,r2}
	cmp		r0, #0
	bne		finalFunc2
	
	sub		r12, r12, #1
	cmp 	r12, #0
	movle 	r0, #1
	ble 	salirFor
	
	;encontrada, entonces avanzar índices
	add		r1, r1, r4				; r1 = nueva_fila = fila + delta_fila
	add		r2, r2, r5				; r2 = nueva_columna = columna + delta_columna
	
	;incrementar longitud y visitar celda vecina
	mov		r0, r7
	mov		r3, r10
	b 		buscar_alineamiento2	; Saltar a funcion
	
finalFunc2
	; Recuperar valores
	mov		r0, r7
	mov		r1, r8
	mov		r2, r9
	mov		r3, r10
	
	add		r6, r6, #8
	cmp     r6, #32
    blt   	inicioBucle
	mov		r0, #0
	
salirFor
	ldmia	r13!,{r4-r10,r12,r11,pc}

	END