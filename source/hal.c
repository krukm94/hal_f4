#include "hal.h"


//=================================================
void halLib_Init(void){
   SystemInit();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
   
	 uart_init();
   // Przywitanie
		uart_puts("\n\r---> STM32F4 <----\n\r");
		uart_puts(__TIME__);
		
		TIM4_init();
		led_port_init();
	
   // lcdInit();
	
	// HAL->accInit();
	
	 //HAL->accSetDataRate(LIS3DSH_ODR_100_HZ);
}

//=================================================
const t_halLib halLib /*__attribute__((at(0x8010100)))*/ ={
   //system
   halLib_Init,               //void     (*Init)(void);
   NVIC_SystemReset,          //void     (*mcuRst)(void);
   0,                         //void     (*mcuGetID)(char *id25char);
   timerSetIrqFun,                         //int8_t  (*timerSetIrqFun)(uint8_t nrTim, void (*tim_Irq)(void), uint32_t timeMs, uint32_t priority);
   0,                         //void     (*dbgWriteData)(char data);
   //UART 
   uartWrite,                 //void     (*uartWriteData)(char data);
   setIrqFunc,                //void     (*uartIrqSetRecvFun)(void (*fRcv)(char data));
   //LCD
   0,                         //void     (*lcdWrite)(uint8_t xPoz, uint8_t yPoz, char *txt);
   //Klawiatura
   0,                         //void     (*keyboardInit)(uint32_t *pKeyFlag);
   0,                         //void     (*keyIrqSetRecFun)(void (*fKey)(uint32_t keyFlag));
   //Diody
   0,                         //void     (*ledWriteData)(uint8_t data);
   ledOn,                     //void     (*ledOn)(uint8_t nrLed);
   ledOff,                    //void     (*ledOff)(uint8_t nrLed);
   ledToggle,                 //void     (*ledToggle)(uint8_t nrLed);
   //dyski - kartaSD
   0,                         //int32_t  (*discInit)(uint8_t pdrv);
   0,                         //int32_t  (*discRead)(uint8_t pdrv, uint8_t *buff, uint32_t sector, uint32_t count);
   0,                         //int32_t  (*discWrite)(uint8_t pdrv, uint8_t *buff, uint32_t sector, uint32_t count);
   0,                         //int32_t  (*discIoctl)(uint8_t pdrv, uint8_t ctr, uint8_t *buff);
   0,                         //int32_t  (*sdIoctl)(uint8_t ctrl, uint8_t *buff);
	 //delay systick
	 delay_ms,									//void 		(*delay_ms)(int ms);
	 delay_us,									//void 		(*delay_us)(int us);
	 //accelerometer
	 lis3dsh_init,							//void 		(*accInit)(void);
	 set_OutputDataRate,				//void 		(*accSetDataRate)(lis3dsh_register odr_selection);
	 AxesEnable,								//void 		(*accAxesEnable)(lis3dsh_register axes);
	 ReadAxes,									//void 		(*accReadAxes)(LIS3DSH_OutXYZTypeDef* axes);
 
/*
//terminal
	 tr_cursor_hide,						//void		(*tr_cursor_hide)( uint8_t hide );
	 tr_cls,										//void  	(*tr_cls)(uint8_t cur_onoff);
	 tr_locate,									//void 		(*tr_locate)( uint8_t y, uint8_t x );;-
	 tr_pen_color,							//void 		(*tr_pen_color)( uint8_t cl );
	 tr_brush_color,						//void 		(*tr_brush_color)( uint8_t cl );
	 tr_attr,										//void 		(*tr_attr)( uint8_t atr, uint8_t fg, uint8_t bg );
*/
};
t_halLib *HAL=(t_halLib *)&halLib;
