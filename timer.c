#include "timer.h"
#include  "gpio.h"

uint16_t period = 0;
uint8_t lcdBrightness = 10;

/**
 * Метод настройки таймера 3 в режиме прерываний по переполнению.
 */
void initTimer3() {
	timerConf(RCC_APB1Periph_TIM3, 1000, 10, TIM3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * Метод настройки таймера 2 в режиме прерываний по переполнению.
 */
void initTimer2() {
	timerConf(RCC_APB1Periph_TIM2, 1000, 500, TIM2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * Метод, содержащий базовые настройки таймера, для его настройки в режиме прерываний по переполнению.
 * RCC_APB1Periph - шина, тактирующая данный таймер.
 * prescaler      - значение предделителя частоты таймера (частота в Гц).
 * iTperiod		  - частота прерываний (prescaler/iTperiod) Гц.
 * TIMx			  - таймер, который необходимо проинициализировать данными настройками.
 */
void timerConf(uint32_t RCC_APB1Periph, uint16_t prescaler, uint16_t iTperiod, TIM_TypeDef* TIMx) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);
	BaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / prescaler) - 1;
	BaseStructure.TIM_Period = iTperiod;
	TIM_TimeBaseInit(TIMx, &BaseStructure);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); //Активируем прерывание по переполнению
	TIM_Cmd(TIMx, ENABLE);
}

/**
 * Метод настройки таймера 1 в режиме ШИМ генерации.
 */
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

/**
 * Метод задания коэффициента заполнения (0 - 100) ШИМ таймера 1.
 */
void setBrightness(uint8_t brightness) {
	TIM1->CCR4 = (uint16_t)(((uint32_t) brightness * (period - 1)) / 100);
}

/**
 * Метод возвращает установленное значение яркости дисплея (0 - 100).
 */
uint8_t getLcdBrightness() {
	return lcdBrightness;
}

/**
 * Метод установки яркости дисплея (0 - 100).
 */
void setLcdBrightness(uint8_t brightness) {
	lcdBrightness = brightness;
	setBrightness(brightness);
}
