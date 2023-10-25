                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "Power_management.h"
#include "gpio_hal.h"
#include "temporizador_drv.h"
#include "fifo.h"
#include "io_reserva.h"
#include "planificador.h"

int main (void) {
	planificador();
	while (1);
}
