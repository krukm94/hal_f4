#include "delay.h"

void delay_ms(int ms)
{
	int c = 0;

	SysTick -> LOAD = SYSTICK_F/1000 - 1;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;

	while(c < ms)
	{
		while(!(SysTick -> CTRL & SysTick_CTRL_COUNTFLAG_Msk));
		c++;
	}
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delay_us(int us)
{
	int c = 0;

	SysTick -> LOAD = SYSTICK_F/1000000 - 1;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;

	while(c < us)
	{
		while(!(SysTick -> CTRL & SysTick_CTRL_COUNTFLAG_Msk));
		c++;
	}
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
