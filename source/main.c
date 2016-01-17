#include "hal.h"
#include "test.h"
#include "mk_term.h"
#include "uart.h"

/* --------------> VARIABLES */
extern volatile uint32_t procent;


int main (void)
{
	// Inicjalizacja naszego HALA
	HAL->Init();
		
	while(1)
	{
		// zmienna do wyliczania obciazenia uC
		procent++;

	}
}

