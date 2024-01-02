/**
 * Autor: Morales Rosa, Lucia (816906) y Pav�n Calcerrada, Claudia (852698)
 * Practica 2 - Proyecto Hardware
 * 				swi.h
*/

#ifndef SWI_H
#define SWI_H

#include <stdint.h>

/**
 * Lee el estado del bit IRQ (Interrupción de solicitud) mediante un SWI.
 *
 * @return El estado del bit IRQ (0 o 1).
 */
uint32_t __swi(0xFC) read_IRQ_bit(void);

/**
 * Deshabilita las interrupciones de IRQ mediante un SWI.
 */
void __swi(0xFD) disable_fiq(void);

/**
 * Habilita las interrupciones de IRQ mediante un SWI.
 */
void __swi(0xFF) enable_irq(void);

/**
 * Deshabilita las interrupciones de IRQ mediante un SWI.
 */
void __swi(0xFE) disable_irq(void);

#endif
