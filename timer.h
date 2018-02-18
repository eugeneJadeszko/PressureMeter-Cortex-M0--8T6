#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"

TIM_TimeBaseInitTypeDef BaseStructure;
TIM_OCInitTypeDef OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

void initTimer3();
void initTimer2();
void timerConf(uint32_t RCC_APB1Periph, uint16_t prescaler, uint16_t iTperiod, TIM_TypeDef* TIMx);
void initPWMtimer1();
void setBrightness(uint8_t brightness);
uint8_t getLcdBrightness();
void setLcdBrightness(uint8_t brightness);

#endif
