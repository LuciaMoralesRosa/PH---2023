                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
#include "gpio_hal.h"
#include "temporizador_drv.h"
#include "fifo.h"
#include "io_reserva.h"
#include "planificador.h"

void encolar(void) {
	//FIFO_encolar(ID_VOID, 0);
	T1IR = 1;                              // Clear interrupt flag
	VICVectAddr = 0;                            // Acknowledge Interrupt
}

int main (void) {
 	FIFO_inicializar(GPIO_OVERFLOW);
	timer1_drv_reloj(500, encolar);
	planificador();
	while (1);
}
