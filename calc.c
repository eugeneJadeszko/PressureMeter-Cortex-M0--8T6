#include "calc.h"
#include "adc.h"
#include "utility.h"
#include "status.h"

uint16_t u16_result[2];
uint16_t adc_buf_a[COUNT_FILTER];
uint16_t adc_buf_b[COUNT_FILTER];
FILTER_REG reg_a;
FILTER_REG reg_b;

/**
 * Метод реализует алгоритм усреднения "скользящая средняя".
 * adc_val    - значение АЦП.
 * buf        - указатель на массив, хранящий значения преобразований.
 * filter_reg - указатель на структуру, определяющую параметры агоритма.
 */
static uint16_t filter_sred(uint16_t adc_val, uint16_t* buf, FILTER_REG* filter_reg) {
	if (filter_reg->Reg.Flag) {
		filter_reg->Reg.Filter_sum -= buf[filter_reg->Reg.Index];
		filter_reg->Reg.Filter_sum += adc_val;
		buf[filter_reg->Reg.Index] = adc_val;
		if (filter_reg->Reg.Index >= COUNT_FILTER - 1) {
			filter_reg->Reg.Index = 0;
		} else {
			filter_reg->Reg.Index++;
		}
	} else {
		filter_reg->Reg.Filter_sum += adc_val;
		buf[filter_reg->Reg.Index] = adc_val;
		if (filter_reg->Reg.Index >= COUNT_FILTER - 1) {
			filter_reg->Reg.Index = 0;
			filter_reg->Reg.Flag = 1;
		} else {
			filter_reg->Reg.Index++;
		}
	}
	return (filter_reg->Reg.Filter_sum >> COUNT_RSHIFT);
}

/**
 * Метод обновляет значения давления и заряда АКБ.
 */
void updateValues() {
	updatePressureValue();
	updateChargeValue();
}

/**
 * Метод преобразует показания АЦП в значение давления (кПа)
 * и записывает полученное значение в переменную.
 */
void updatePressureValue() {
	u16_result[0] = filter_sred(getAdcValue(), adc_buf_a, &reg_a);
	if (u16_result[0] > 1540) {
		setPressure(0);
	} else if (u16_result[0] < 810) {
		setPressure(600);
	} else if ((1112 > u16_result[0]) && (u16_result[0] >= 810)) {
		setPressure(-0.98f * u16_result[0] + 1340);
	} else if ((1540 >= u16_result[0]) && (u16_result[0] >= 1112)) {
		setPressure(-0.58f * u16_result[0] + 890);
	}
}

uint8_t chargeValue;
/**
 * Метод преобразует показания АЦП в значение зарада батареи (проценты)
 * и записывает полученное значение в переменную.
 */
void updateChargeValue() {
	u16_result[1] = filter_sred(getAdcValue(), adc_buf_b, &reg_b);
	if (u16_result[1] > 1706) {
		chargeValue = (uint8_t)((float) (u16_result[1] - 1706) / 6.2f);
	} else {
		setCharge(0);
	}
	if (chargeValue > 100) {
		setCharge(100);
	} else {
		setCharge(chargeValue);
	}
}
