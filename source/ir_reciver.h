/* 		Biblioteka do obsBugi odbiorników podczerwieni z seri TSOP 

			Nie jestem pewny lini 139 w pliku C i zmian reakcji na zbocze opadajace i narastajace
			Na razie na sztywno 
			
			Wszystkie wartosci binarne zamieniac na hex!
Autor: MK
DATA : 27.08.2015

Aktualizacja: 12.09.2015 11:52

*/

#ifndef __ir_reciver_H__
#define __ir_reciver_H__


//--------------------------------------------------------------------------
/* includes */
#include "stdint.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "uart.h"

//---------------------------------------------------------------------------

/* macros */
#define TIMER_PRESCALER 1
#define F_CPU 42000000

//przeliczanie tykniec Timera1 przy danym preskalerze na mikrosekundy
#define ir_micro_s(num) ((num)*(F_CPU/1000000)/TIMER_PRESCALER)
//stale i zmienne potrzebne na wewnetrzne cele procedury obslugi RC5
//stale czasowe i tolerancja wyrazone w us
#define TOLERANCE 200
#define MIN_HALF_BIT			ir_micro_s(899 - TOLERANCE)
#define MAX_HALF_BIT 			ir_micro_s(889 + TOLERANCE)
#define MAX_BIT						ir_micro_s((889+889)+ TOLERANCE)


//-------------------------------------------- definicje klawiszy Pilota IR
#define teletext_red 55
#define teletext_green 54
#define teletext_yellow 52
#define menu 36
#define button_ok 59
#define power_off 12
#define mute 13
#define vol_up 16
#define vol_down 17
#define prog_up 32
#define prog_down 33
#define cursor_up prog_up
#define cursor_down prog_down
#define cursor_left vol_down
#define cursor_right vol_up

//---------------------------------------------------------------------------

/*variables */
extern volatile uint8_t address;		//adres RC5
extern volatile uint8_t command;		//komenda RC5
extern volatile uint8_t toggle_bit;	//bit Toggle

/* flaga = 1 - informuje, ze z pilota zostal odebrany nowy kod,
 * po o=odczytaniu danych ze zmiennych nalezy wyzerowac flage, aby
 * zostaly przyjete kolejne kody z pilota
 */

extern volatile uint8_t Ir_key_press_flag;
extern volatile uint8_t rc5cnt;

//---------------------------------------------------------------------------
/*functions */
void		 ir_init(void);

#endif // __ir_reciver_H__

