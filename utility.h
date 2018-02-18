#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <stm32f0xx.h>

void delay_us(uint32_t t);

void delay_ms(uint32_t t);

uint8_t ConvertIntToString(int n, char* s);
uint8_t ConvertFloatToString(float f, char* s);

int32_t ConvertStringToInt(char* s, uint8_t pos);
float ConvertStringToFloat(char* s, uint8_t pos);

#endif
