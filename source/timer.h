/* Plik naglowkowy od funkcji ktora wywolana inicjuje timer tak aby w przerwaniach po przepelnieniu wywlywal funkcje
wskaznik na wywalowana funkcje jest jednym z argumentów. 

funkcja timerSetIrqFun dziala poprawnie przy max czestotliwosci taktujacej

Autor: Michal Kruk/
Data : 20.07.2015

*/

#ifndef __timer_H__
#define __timer_H__


/* Includes */
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stdint.h"
#include "itoa.h"
#include "uart.h"
#include "led.h"
#include "mk_term.h"

/* macros */

//f_apb = czestotliwosc taktowania szyny 
#define F_APB1 42000000
#define F_APB2 84000000




#define RCC_APB1Periph_(par) 		(((par) == 2) || \
                                   ((par) == 3) || \
                                   ((par) == 4) || \
                                   ((par) == 5) || \
                                   ((par) == 6) || \
                                   ((par) == 7) || \
                                   ((par) == 12) || \
                                   ((par) == 13) || \
                                   ((par) == 14)))



/*functions*/
uint8_t timerSetIrqFun(uint8_t nrTim, void (*tim_Irq)(void), uint32_t timeMs, uint32_t priority);

uint8_t TIM4_init(void);

void obciazenie_uC(void);
#endif // __timer_H__

