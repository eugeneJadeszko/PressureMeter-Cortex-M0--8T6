#ifndef _ADC_H_
#define _ADC_H_

#include <stm32f0xx_adc.h>
#include <inttypes.h>

ADC_InitTypeDef ADC_InitStructure;

void adcInit(void);
uint16_t getAdcValue();

#endif
