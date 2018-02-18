#ifndef _CALC_H_
#define _CALC_H_

#include "inttypes.h"

#define COUNT_FILTER    512
#define COUNT_RSHIFT    9

typedef union {
	uint32_t Val;
	struct {
		uint32_t Flag :1; //флаг заполнения суммы
		uint32_t Index; //хранение индекса буфера
		uint32_t Filter_sum; //регистр суммы для усреднения
	} Reg;
} FILTER_REG;

void updateValues();
void updatePressureValue();
void updateChargeValue();

#endif
