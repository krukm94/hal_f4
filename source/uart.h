#ifndef __uart_H__
#define __uart_H__

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include <string.h>

#define APB1_CLK   42000000
#define BAUDRATE   115200

#define baudrate 115200

void uart_init(void);
void uartWrite(char dana);
void setIrqFunc(void (*pFunc)(char dana));
void uart_putc(char data);
void uart_puts(char * s);
void uart_putint(uint32_t liczba, uint8_t radix);

#endif // __uart_H__

