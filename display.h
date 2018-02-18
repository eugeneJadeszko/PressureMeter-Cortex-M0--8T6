#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "inttypes.h"
#include "stm32f0xx_tim.h"
#include "buttons.h"

void DisplayInit(void);
void Display(void);
void mainDisplay();
void menu();
void itemMaxThreshold();
void itemMinThreshold();
void itemBrightnessControl();
uint8_t quit();
void brightnessControl();
void dehighLighting(int8_t *step, uint8_t lcdBrightness);
void highLighting(int8_t *step, uint8_t lcdBrightness);

void setMinThreshold();
void setMaxThreshold();
uint16_t getMaxThreshold();
uint16_t getMinThreshold();

void IconDraw(uint8_t number, uint8_t * char_data);
void BatteryIconDraw(void);

#endif
