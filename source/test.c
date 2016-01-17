#include <string.h>
#include "test.h"
#include "hal.h"


LIS3DSH_OutXYZTypeDef odczyt;

void diody_test(void)
{
	HAL->ledOn(12);
	HAL->ledOn(13);
	HAL->ledOn(14);
	HAL->ledOn(15);
}

void acc_test(void)
{
	HAL->accReadAxes(&odczyt);

	if(odczyt.x>5000)
		HAL->ledOn(14);
	else HAL->ledOff(14); //red
	
	if(odczyt.x<-5000)
		HAL->ledOn(12);
	else HAL->ledOff(12); //green
		
	if(odczyt.y>5000)
		HAL->ledOn(13);
	else HAL->ledOff(13); //orange
	
	if(odczyt.y<-5000)
		HAL->ledOn(15);
	else HAL->ledOff(15); //blue
}

