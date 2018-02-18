#include "calc.h"
#include "adc.h"
#include "utility.h"
#include "status.h"

uint16_t u16_result[2];
uint16_t adc_buf_a[COUNT_FILTER];
uint16_t adc_buf_b[COUNT_FILTER];
FILTER_REG reg_a;
FILTER_REG reg_b;

static uint16_t filter_sred(uint16_t adc_val, uint16_t* buf,
		FILTER_REG* filter_reg) {
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

void updateValues() {
	updatePressureValue();
	updateChargeValue();
}

void updatePressureValue() {
	u16_result[0] = filter_sred(getAdcValue(), adc_buf_a, &reg_a);
	if (u16_result[0] > 1360) {
		setPressure(0);
	} else {
		setPressure(-1.04f * u16_result[0] + 1425);
	}
}

uint8_t chargeValue;
void updateChargeValue() {
	u16_result[1] = filter_sred(getAdcValue(), adc_buf_b, &reg_b);
	if (u16_result[1] > 1706) {
		chargeValue = (uint8_t)((float) (u16_result[1] - 1706) / 5.7f);
	} else {
		setCharge(0);
	}
	if (chargeValue > 100) {
		setCharge(100);
	} else {
		setCharge(chargeValue);
	}
}
