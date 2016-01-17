#ifndef DELAY_SYSTICK_DELAY_SYSTICK_H_
#define DELAY_SYSTICK_DELAY_SYSTICK_H_

#include "stm32f4xx.h"

#define SYSTICK_F 21000000

void delay_ms(int ms);
void delay_us(int us);


#endif /* DELAY_SYSTICK_DELAY_SYSTICK_H_ */
