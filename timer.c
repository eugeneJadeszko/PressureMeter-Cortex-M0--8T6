#include "timer.h"
#include  "gpio.h"

uint16_t period = 0;
uint8_t lcdBrightness = 10;

void initTimer3() {
	timerConf(RCC_APB1Periph_TIM3, 1000, 10, TIM3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void initTimer2() {
	timerConf(RCC_APB1Periph_TIM2, 1000, 500, TIM2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void timerConf(uint32_t RCC_APB1Periph, uint16_t prescaler, uint16_t iTperiod, TIM_TypeDef* TIMx) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);
	BaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / prescaler) - 1; //Делитель на prescaler Гц
	BaseStructure.TIM_Period = iTperiod; // Частота прерываний: (prescaler/iTperiod) Гц
	TIM_TimeBaseInit(TIMx, &BaseStructure);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); //Активируем прерывание по переполнению
	TIM_Cmd(TIMx, ENABLE);
}

void initPWMtimer1() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	period = (uint16_t)(SystemCoreClock / 1000) - 1;
	BaseStructure.TIM_Prescaler = 0;
	BaseStructure.TIM_Period = period;
	TIM_TimeBaseInit(TIM1, &BaseStructure);
	OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; //dead time тип
	OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	OCInitStructure.TIM_Pulse = (uint16_t)((lcdBrightness * (period - 1)) / 100); //Вычисление коэффициента заполнения
	TIM_OC4Init(TIM1, &OCInitStructure);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

void setBrightness(uint8_t brightness) {
	TIM1->CCR4 = (uint16_t)(((uint32_t) brightness * (period - 1)) / 100);
}

uint8_t getLcdBrightness() {
	return lcdBrightness;
}

void setLcdBrightness(uint8_t brightness) {
	lcdBrightness = brightness;
	setBrightness(brightness);
}
