#include "uart.h"
#include "itoa.h"

volatile uint8_t temp;
volatile char data;
static void (*uartRE)(char data);

volatile uint8_t watch;

void uart_init(void) 
{
	GPIO_InitTypeDef     GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
 
// Enable clock for GPIOA
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 
// Enable clock for USART1 peripheral
 
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	

	
// Initialize pins as alternating function
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
/**
* Tell pins PA9 and PA10 which alternating function you will use
* @important Make sure, these lines are before pins configuration!
*/
GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //USART2_TX
GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);	//USART2_RX
 
/**
 * Set Baudrate to value you pass to function
 * Disable Hardware Flow control
 * Set Mode To TX and RX, so USART will work in full-duplex mode
 * Disable parity bit
 * Set 1 stop bit
 * Set Data bits to 8
 *
 * Initialize USART2
 * Activate USART2
 */
 
USART_InitStruct.USART_BaudRate = baudrate;
USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
USART_InitStruct.USART_Parity = USART_Parity_No;
USART_InitStruct.USART_StopBits = USART_StopBits_1;
USART_InitStruct.USART_WordLength = USART_WordLength_8b;
USART_Init(USART2, &USART_InitStruct);

USART_Cmd(USART2, ENABLE); 

/**
 * Enable RX interrupt
 */
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 
/**
 * Set Channel to USART2
 * Set Channel Cmd to enable. That will enable USART2 channel in NVIC
 * Set Both priorities to 0. This means high priority
 *
 * Initialize NVIC
 */
		NVIC_SetPriority(USART2_IRQn, 0x01);
		NVIC_EnableIRQ (USART2_IRQn);
	
}

void uartWrite(char data)
{
	USART_SendData(USART2, data);

}


void uart_putc(char data){
		while(!(USART2->SR & USART_SR_TXE)); 
		USART_SendData(USART2, data);
		watch =1;
}

void uart_puts(char * s)
{
	while (*s)
	{
		uart_putc(*s++);
	}
}

void uart_putint(uint32_t liczba, uint8_t radix) {

	char buf[17];
	itoa(liczba, buf, radix);
	uart_puts(buf);
}


void setIrqFunc(void (*pFunc)(char data))
{
	if(pFunc)
	{
		uartRE = uartWrite;
		
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	}
	else
	{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
	
		uartRE = NULL;
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
		temp = USART_ReceiveData(USART2);
		
		uartRE(temp);
		
	}
}
