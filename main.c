#include "gpio.h"
#include "Timer.h"
#include "hd44780.h"
#include "display.h"
#include "buttons.h"
#include "adc.h"
#include "status.h"
#include "calc.h"

/**
 * Метод, с которого начинается выполение программы.
 * Вызывает методы инициализации и содержит основной цикл программы.
 */
int main(void) {
	GPIOInit();
	initPWMtimer1();
	initTimer2();
	DisplayInit();
	adcInit();
	initTimer3();

	while (1) {
		Display();
	}
}

/**
 * Метод обработки прерываний TIM3.
 * Осуществляет обновление значений давления и заряда АКБ,
 * а так же обработку состояния кнопки включение/отключения подсветки дисплея.
 */
void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		updateValues();
		buttonLcdLed();
	}
}

/**
 * Метод обработки прерываний TIM2.
 * Осуществляет обновление показаний на дисплее, таких как:
 * состояние АКБ, значение давления и состояние управляемого устройства.
 */
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		updateStatuses();
	}
}
