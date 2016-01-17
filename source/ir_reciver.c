/* ir_reciver   
Projekt chwilowo na sztywno pod okreslony pin PD13
*/
#include "ir_reciver.h"

volatile uint8_t address;
volatile uint8_t command;		//komenda RC5
volatile uint8_t toggle_bit;	//bit Toggle
volatile uint8_t Ir_key_press_flag;
volatile uint8_t rc5cnt;



void ir_init(void){
	
	/* GPIO,Tim structure initialize */
	GPIO_InitTypeDef     GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef timerInitStructure; 
	
	/* set RCC on for GPIO port and TIM4 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	/*GPIO configuration for IR PORT */
	/* The most important is set:
		IR PIN as in
		IR PIN pull up
	*/
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	/* Timer4 configuration  Timer works in external mode 	*/
	

/*  														Ustawienie TIM4 w trybie slave external   - tak dla sciagi (mój pierwsz projekt z tim w trybie external


																 TIM4 -> CCMR1 |= TIM_CCMR1_CC2S_0;									// Ustawiamy kanal 2 na wykrywanie narastajacego zbocza
																 TIM4 -> SMCR  |= TIM_SMCR_SMS;											//ustawiamy timer w external clock mode 1
																 TIM4 -> SMCR  |= TIM_SMCR_TS_1|TIM_SMCR_TS_2;			//ustawiamy TI2 jako wejcie
																 TIM4 -> DIER  |= TIM_DIER_UIE;											// wlaczamy przerwania od update
																 TIM4 -> CR1   |= TIM_CR1_CEN;											//Wlaczamy licznik
																	 TIM4 -> ARR   = 9;
																	*/
		
		

	
			/*Configures the TIMx Trigger as External Clock
			This function set : clock mode 
													Trigger source
													filter value
													detected edge */
			TIM_TIxExternalClockConfig(TIM4,TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Falling,0x0);			
																//nr tim , zródlo ,                , zobcze              , wartosc filtru 


			/*standard option to configuration */
			
			timerInitStructure.TIM_Prescaler = TIMER_PRESCALER;	
			timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;							// Zliczanie w góre													
			timerInitStructure.TIM_Period = 9;																		// Pod Period kryje sie ustawianie rejestru ARR  -- chwilowo nie skupiam sie na ustawieniu dokladnej wartosci
			timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;					
			timerInitStructure.TIM_RepetitionCounter = 0;
			TIM_TimeBaseInit(TIM4, &timerInitStructure);
			TIM_Cmd(TIM4, ENABLE);
	
			/* NVIC configuration */
			NVIC_SetPriority(TIM4_IRQn, 0);
			NVIC_EnableIRQ (TIM4_IRQn);
			
			/* Enables or disables the specified TIM interrupts. */
			
			TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);																	//This function set IRQ from TIM4 enable																	
							//nr TIM, rodzaj przerwania , enable /disable 	
							
		/*zerowanie licznika wystepujacych zboczy */
			rc5cnt = 0;
			/*zerowanie flagi otrzymania kodu z pilota */
			Ir_key_press_flag = 0;
}

//-------------------------------------------------------------------------------------------------------

/* przerwanie od CC2 ( capture compare )*/
	
void TIM4_IRQHandler(void){

			if(TIM_GetITStatus(TIM4, TIM_IT_Update ) != RESET){
				
				static uint16_t LastCapture;
				uint16_t PulseWitdh;

				static uint8_t IrPulseCount;
				static uint16_t IrData;
				static uint8_t frame_status;
					
				
				TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			
			
				#define FRAME_RESTART 0
				#define FRAME_OK 1
				#define FRAME_END 2
				#define FRAME_ERROR 3
				
				
				
				/* -------------- */
				PulseWitdh = TIM_GetCounter(TIM4) - LastCapture;
				LastCapture = TIM_GetCounter(TIM4);
				/* zmiana zbocza na przeciwne */
				//TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising);
				TIM4 -> SMCR ^= TIM_SMCR_ETP;
				
				
				if(PulseWitdh > MAX_BIT) rc5cnt = 0;
				
				if(rc5cnt > 0) frame_status = FRAME_OK;
				
					if(rc5cnt == 0) {
						IrData = 0;
						IrPulseCount = 0;
							
						/* zmiana zbocza na przeciwne */
						//TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Falling);
						TIM4 -> SMCR ^= TIM_SMCR_ETP;
						/* ------------------------- */
						rc5cnt++;
						frame_status = FRAME_END;
					}
					
						if (frame_status == FRAME_OK) {
							//gdy zaklócenia (szpilki) - RESTART
							
							if(PulseWitdh < MIN_HALF_BIT) frame_status = FRAME_RESTART;
							
								//gdy blad ramki danych  (moze inny standard niz RC5) - RESTART
								if(PulseWitdh > MAX_BIT ) frame_status = FRAME_RESTART;
							
									if(frame_status == FRAME_OK) {
										if(PulseWitdh > MAX_HALF_BIT) rc5cnt++;
				
											if (rc5cnt > 1)
												if ((rc5cnt % 2) == 0 ){
													IrData = IrData << 1;
													if((TIM4 -> SMCR & TIM_SMCR_ETP)==0) IrData |= 0x0001;		//Ta instrukcja ma sprawdzac czy jest narastajace zbocze obecnie
													IrPulseCount++;
													if(IrPulseCount > 12) {
														if (Ir_key_press_flag == 0) {
															command = (IrData & 0x3F);					//0b0000000000111111
															address = (IrData & 0x7C0 >> 6);		//0b0000011111000000
															toggle_bit = (IrData & 0x800 >> 11);			//0b0000100000000000
														}
													frame_status = FRAME_RESTART;
													Ir_key_press_flag = 1;
													}
												}
												rc5cnt++;
									}
								}
								
							if (frame_status == FRAME_RESTART) {
								rc5cnt = 0;
								/* zmiana zbocza na przeciwne */
								//TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising);		
								TIM4 -> SMCR ^= TIM_SMCR_ETP;
							}
						}
}	/* irq instructions end */ 
								

