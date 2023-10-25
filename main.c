                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
#include "gpio_hal.h"
#include "temporizador_drv.h"
#include "fifo.h"
#include "io_reserva.h"
#include "planificador.h"
#include "int_externas_hal.h"

int main (void) {
	gpio_hal_iniciar();
 	eint1_init();
 	eint2_init();
	while (1) {
		if (devolver()==1) {
			gpio_hal_escribir(GPIO_OVERFLOW,GPIO_OVERFLOW_BITS,0x0);
		}
		else if (devolver()==2){
			gpio_hal_escribir(GPIO_OVERFLOW,GPIO_OVERFLOW_BITS,0x1);
		}
	}
}
