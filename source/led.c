#include "led.h"


void led_port_init(void)
{
	//wlaczenie zegara portu D
	RCC->AHB1ENR |= (1 << 3);

	//inicjalizacja GPIO D15, D14, D13, D12
	GPIOD->MODER |= (1 << 30) | (1 << 28) | (1<<26) | (1<<24);
	GPIOD->OSPEEDR |= (1 << 30) | (1<<28) | (1<<26) | (1<<24);

	//resetowanie stanów wyjsc D15, D14, D13, D12
	GPIOD->ODR &= ~(1 << 15) | ~(1 << 14) | ~(1 << 13) | ~(1 << 12);
}

int8_t  ledOn( uint8_t nrLed )
{
	
	if(nrLed<12||nrLed>15) return (1);
	GPIOD->BSRRL = (1<<nrLed);
	return 0;
}

int8_t ledOff( uint8_t nrLed )
{
	if(nrLed<12||nrLed>15) return (1);
	GPIOD->BSRRH = (1<<nrLed);
	return 0;
}

int8_t  ledToggle( uint8_t nrLed )
{
	if(nrLed<12||nrLed>15) return (1);
	GPIOD->ODR ^= (1<<nrLed);
	return 0;
}
