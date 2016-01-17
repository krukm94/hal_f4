#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx.h"


void led_port_init(void);

int8_t ledWriteData(uint8_t  data); // ?
int8_t ledOn(uint8_t nrLed);
int8_t ledOff(uint8_t nrLed);
int8_t ledToggle(uint8_t nrLed);


#endif
