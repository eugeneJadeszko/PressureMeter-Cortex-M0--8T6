#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "inttypes.h"
#include "gpio.h"

typedef enum {
	IS_PRESSED = 0,
	IS_NOT_PRESSED = !IS_PRESSED,
	PRESSED = 1,
	NOT_PRESSED = !PRESSED,
	ON = 1,
	OFF = !ON
} State;

//struct Button {
//	uint16_t Pin;
//	uint8_t lastState;
//};
//typedef struct Button But;

//uint8_t button(But button);
uint8_t buttonPlus();
uint8_t buttonMenuOk();
void buttonLcdLed();

#endif
