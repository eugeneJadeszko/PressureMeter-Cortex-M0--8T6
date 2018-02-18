#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"

GPIO_InitTypeDef GPIO_InitStructure;

#define DEVICE_OUT_PA7 	GPIO_Pin_7
#define B_PLUS_PA9	 	GPIO_Pin_9
#define B_MENU_OK_PA10  GPIO_Pin_10
#define LCD_LIGHT		GPIO_Pin_11
#define BUTTON_LIGHT 	GPIO_Pin_12

#define ADC0_SENS			GPIO_Pin_0
#define ADC1_BAT			GPIO_Pin_1
#define CHRG				GPIO_Pin_13
#define STDBY				GPIO_Pin_14

void GPIOInit(void);

#endif
