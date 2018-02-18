#include "buttons.h"
#include "gpio.h"
#include "hd44780.h"
#include "display.h"
#include "utility.h"

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

uint8_t lastButtonLightState;
uint8_t lcdLedState = ON;
uint8_t lastStatePlus;
uint8_t lastStateMenuOk;

/**
 * Метод обработки нажатия кнопки.
 */
//uint8_t button(But button) {
//	uint8_t currentState;
//	currentState = GPIO_ReadInputDataBit(GPIOA, button.Pin);
//	if ((currentState == IS_NOT_PRESSED) && (button.lastState == IS_PRESSED)) {
//		button.lastState = IS_NOT_PRESSED;
//		return NOT_PRESSED;
//	} else if ((currentState == IS_PRESSED) && (button.lastState == IS_NOT_PRESSED)) {
//		button.lastState = IS_PRESSED;
//		return PRESSED;
//	}
//	return NOT_PRESSED;
//}

/**
 * Метод обработки нажатия кнопки МЕНЮ_ОК
 */
uint8_t buttonMenuOk() {
	uint8_t currentState;
	currentState = GPIO_ReadInputDataBit(GPIOA, B_MENU_OK_PA10);
	if ((currentState == IS_NOT_PRESSED) && (lastStateMenuOk == IS_PRESSED)) {
		lastStateMenuOk = IS_NOT_PRESSED;
		return NOT_PRESSED;
	} else if ((currentState == IS_PRESSED) && (lastStateMenuOk == IS_NOT_PRESSED)) {
		lastStateMenuOk = IS_PRESSED;
		return PRESSED;
	}
	return NOT_PRESSED;
}

/**
 * Метод обработки нажатия кнопки "+"
 */
uint8_t buttonPlus() {
	uint8_t currentState;
	currentState = GPIO_ReadInputDataBit(GPIOA, B_PLUS_PA9);
	if ((currentState == IS_NOT_PRESSED) && (lastStatePlus == IS_PRESSED)) {
		lastStatePlus = IS_NOT_PRESSED;
		return NOT_PRESSED;
	} else if ((currentState == IS_PRESSED) && (lastStatePlus == IS_NOT_PRESSED)) {
		lastStatePlus = IS_PRESSED;
		return PRESSED;
	}
	return NOT_PRESSED;
}

/**
 * Метод обработки состояния кнопки управления подсветкой экрана
 */
void buttonLcdLed() {
	uint8_t currentState;
	currentState = GPIO_ReadInputDataBit(GPIOB, BUTTON_LIGHT);
	if ((currentState == IS_PRESSED) && (lastButtonLightState == IS_NOT_PRESSED)) {
		if (lcdLedState == ON) {
			lcdLedState = OFF;
			LcdLedOff();
		} else {
			lcdLedState = ON;
			LcdLedOn();
		}
		lastButtonLightState = IS_PRESSED;
	} else if ((currentState == IS_NOT_PRESSED) && (lastButtonLightState == IS_PRESSED)) {
		lastButtonLightState = IS_NOT_PRESSED;
	}
}
