#ifndef __hal_H__
#define __hal_H__

#include <stdint.h> 
#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "misc.h"
#include "lis3dsh.h"
#include "uart.h"
#include "timer.h"

//#include "mk_term.h"

//#include "lcd.h"

#define _userPriorytyOffset     6

#define _hal_uart1_Prioryty     1
#define _hal_KeyIrq_Prioryty    4

#define _nrUserTimer            4


typedef struct{
   //system
   void     (*Init)(void);
   void     (*mcuRst)(void);
   void     (*mcuGetID)(char *id25char);
   uint8_t	(*timerSetIrqFun)(uint8_t nrTim, void (*tim_Irq)(void), uint32_t timeMs, uint32_t priority);
   void     (*dbgWriteData)(char data); // - pusta
   //UART 
   void     (*uartWriteData)(char data);
   void     (*uartIrqSetRecvFun)(void (*fRcv)(char data));
   //LCD
   int32_t  (*lcdWrite)(uint8_t xPoz, uint8_t yPoz, char *txt);
   //Klawiatura
   void     (*keyboardInit)(uint32_t *pKeyFlag);
   void     (*keyIrqSetRecFun)(void (*fKey)(uint32_t keyFlag));
   //Diody
   int8_t  (*ledWriteData)(uint8_t data);
   int8_t  (*ledOn)(uint8_t nrLed);
   int8_t  (*ledOff)(uint8_t nrLed);
   int8_t  (*ledToggle)(uint8_t nrLed);
	
   //dyski - kartaSD
   int32_t  (*sdInit)(void);
   int32_t  (*sdStatus)(void);
   int32_t  (*sdRead)(uint8_t *buff, uint32_t sector, uint32_t count);
   int32_t  (*sdWrite)(uint8_t *buff, uint32_t sector, uint32_t count);
   int32_t  (*sdIoctl)(uint8_t ctrl, uint8_t *buff);
	 //delay systick
	 void 		(*delay_ms)(int ms);
	 void 		(*delay_us)(int us);
	 //accelerometer
	 void 		(*accInit)(void);
	 void 		(*accSetDataRate)(lis3dsh_register odr_selection);
	 void 		(*accAxesEnable)(lis3dsh_register axes);
	 void 		(*accReadAxes)(LIS3DSH_OutXYZTypeDef* axes);
/*	 //terminal
	 void			(*tr_cursor_hide)( uint8_t hide );
	 void 		(*tr_cls)(uint8_t cur_onoff);
	 void 		(*tr_locate)( uint8_t y, uint8_t x );
	 void 		(*tr_pen_color)( uint8_t cl );
	 void 		(*tr_brush_color)( uint8_t cl );
	 void 		(*tr_attr)( uint8_t atr, uint8_t fg, uint8_t bg );
*/
}t_halLib;
extern t_halLib *HAL;
//#define HAL ((t_evblLib *) 0x8010100)

#endif // __hal_H__
