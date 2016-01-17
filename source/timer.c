/* Plik C
Autor: Michal Kruk
Data : 20.07.2015
*/

#include "timer.h"

volatile uint32_t procent;
volatile uint32_t procent_save;

char buf[16];
/*  wskaznik na funkcje --------------------------------------------------------*/
static void (*timerIRQ)(void);


/* ----------------------------------------------------------------------------
		TIM4		APB1 f = 82 MHz (only for Timers)
	-------------------------------------------------------------------------------
*/
uint8_t TIM4_init(void){

		//definicja struktury inicjalizacyjnej
		TIM_TimeBaseInitTypeDef TIM4_InitStruct;
	
		//SET CLOCK ON
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		
		//wypelnienie struktury
		TIM4_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM4_InitStruct.TIM_Period 			=	21000 - 1;
		TIM4_InitStruct.TIM_Prescaler		=	4000 - 1;
		
		//Inicjalizacja TIM
		TIM_TimeBaseInit(TIM4, &TIM4_InitStruct);
	
		//NVIC CONFIG
		NVIC_SetPriority(TIM4_IRQn, 0);
		NVIC_EnableIRQ (TIM4_IRQn);
		
		//SET TIM ON
		TIM_Cmd(TIM4, ENABLE);
		
		//SET INTTERUPT ENABLE 
		TIM4 -> DIER |= TIM_DIER_UIE;
		
		return 0;
}
/* ----------------------------------------------------------------------------
		TIM4		PRZERWANIE
	-------------------------------------------------------------------------------
*/
void TIM4_IRQHandler(void){
		if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		ledToggle(12);
		procent_save = procent;
		procent = 0;
			
		obciazenie_uC();

		}
}
/* ----------------------------------------------------------------------------
		Funkcja obliczajaca obciazenie
	-------------------------------------------------------------------------------
*/
void obciazenie_uC(void){
		//obc = (100 - ((procent_save*100)/odniesienie));
		itoa(procent_save, buf, 10);
		
		tr_cls(0);
		tr_locate(0,0);
		uart_puts("\n\rObciazenie uC: ");
		uart_puts(buf);
	
	
}

/* ----------------------------------------------------------------------------
		TIM3
	-------------------------------------------------------------------------------
*/
uint8_t timerSetIrqFun(uint8_t nrTim, void (*tim_Irq)(void), uint32_t timeMs, uint32_t priority){

		/*private variables */
		int32_t presc,f_apb;

		TIM_TimeBaseInitTypeDef timerInitStructure; 

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// Instrukcje warunkowe ustawiaja odpowiednia czestotliwosc szyny do obliczen w zaleznosci od wybranego timera
	// Nalezy pamietac ze Timery na aPB1 maja czestotliwosc razy 2 w przypadku stm32f4
	
	
	
	if(nrTim == 1 || (nrTim > 8 && nrTim < 12 )) {
		f_apb = F_APB2; 
	}		
		if(nrTim > 1 && nrTim < 8){
			f_apb = 2 * F_APB1;
		
		}
			if(nrTim > 11 && nrTim < 15){
				f_apb =2 *  F_APB1;
			}
	

			
	/*Tim configuration -----------------------------------------------------------
	Ponizszy wzor oblicza Prescaler tak aby przerwanie generowane bylo co timeMS.
	-------------------------------------------------------------------------------
	*/
	presc = ((timeMs*f_apb)/1000)-65536-1;																		//w przypadku timera 16 bitoweho
	if(nrTim == 2 || nrTim == 5) presc = ((timeMs*f_apb)/1000)-4294967296-1;  //dla timera 32 bitowego
		
	timerInitStructure.TIM_Prescaler = presc;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//timerInitStructure.TIM_Period = 499;													// Pod Period kryje sie ustawianie rejestru ARR
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	//Tu jeszcze trzeba wymyslec sposob na zrobieniu tego uniwersalnie
	//musialem to zrobic bo przerwanie nie dzialalo

	
	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_Cmd(TIM3, ENABLE);
	
	
	/* ----------------------------------------------------------------------------
		Konfiguracja przerwan dla Timera 
	-------------------------------------------------------------------------------
*/
		NVIC_SetPriority(TIM3_IRQn, priority);
		NVIC_EnableIRQ (TIM3_IRQn);
	
		if(tim_Irq) timerIRQ = tim_Irq;

		TIM3 -> DIER |= TIM_DIER_UIE;    /* WLACZENIE PRZERWANIA MUSI BYC ZA CALBACKIEM1!! ZAWSZE!!! */

		return 0;
}

/* ------------------------------------------------------------------------------
		Przerwanie z Timera
		------------------------------------------------------------------------------
*/




void TIM3_IRQHandler(void){
		if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		

		timerIRQ();
		}
}


