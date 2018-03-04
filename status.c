#include "status.h"
#include "gpio.h"
#include "hd44780.h"
#include "utility.h"
#include "display.h"
#include "buttons.h"

uint8_t charge;
uint16_t pressure;
uint8_t stat;
uint8_t currentDeviceState = ON;

/**
 * Метод обновляет информацию о давлении, состоянии батареи и состоянии управляемого устройства на дисплее.
 */
void updateStatuses() {
	showPressureStatus();
	showBatteryStatus();
	updateDeviceStatus();
}

/**
 * Метод определяет состояние АКБ и обновляет информацию на дисплее.
 */
void showBatteryStatus() {
	uint8_t currentState;
	currentState = getBatteryState();
	if (currentState == CHARGE) {
		showNumberAsPercent(PERCENT_ROW, PERCENT_COL, charge);
		LcdGoToPos(ICON_ROW, ICON_COL);
		stat++;
		if (stat > 6)
			stat = 0;
		LcdSendData(stat);
	} else if (currentState == STANDBY) {
		showNumberAsPercent(PERCENT_ROW, PERCENT_COL, STANDBY);
		LcdSendData(3);
	} else {
		showNumberAsPercent(PERCENT_ROW, PERCENT_COL, charge);
		showChargeAsIcon(ICON_ROW, ICON_COL, charge);
	}
}
/**
 * Метод возвращает текущее состояние батареи.
 *
 * return CHARGE    - если батарея заряжается
 * 		  STANDBY   - если батарея заряжена и подключен кабель зарядки
 * 		  DISCHARGE - если кабель зарядки не подключен
 */
uint8_t getBatteryState() {
	uint8_t isStdby, isChrg;
	isStdby = GPIO_ReadInputDataBit(GPIOB, STDBY);
	isChrg = GPIO_ReadInputDataBit(GPIOB, CHRG);
	if ((isChrg == 0) && (isStdby == 1)) {
		return CHARGE;
	} else if ((isChrg == 1) && (isStdby == 0)) {
		return STANDBY;
	} else {
		return DISCHARGE;
	}
}

/**
 * Метод отображает состояние заряда батареи в виде иконки.
 *
 * row         - строка, в которой будет отображаться иконка батареи
 * column      - столбец, в котором будет отображаться иконка батареи
 * chargeValue - численное значение заряда (0-100)
 */
void showChargeAsIcon(uint8_t row, uint8_t column, uint8_t chargeValue) {
	LcdGoToPos(row, column);
	if (chargeValue >= 95) {
		LcdSendData(6);
	} else if ((chargeValue < 95) && (chargeValue >= 70)) {
		LcdSendData(5);
	} else if ((chargeValue < 70) && (chargeValue >= 50)) {
		LcdSendData(4);
	} else if ((chargeValue < 50) && (chargeValue >= 30)) {
		LcdSendData(3);
	} else if ((chargeValue < 30) && (chargeValue >= 10)) {
		LcdSendData(2);
	} else if ((chargeValue < 10) && (chargeValue >= 5)) {
		LcdSendData(1);
	} else if (chargeValue < 5) {
		LcdSendData(0);
	}
}

/**
 * Метод отображает состояние заряда батареи в процентах.
 *
 * row         - строка, в которой будет отображаться состояние заряда батареи в процентах.
 * column      - столбец, в котором будет отображаться состояние заряда батареи в процентах.
 * chargeValue - численное значение заряда (0-100)
 */
void showNumberAsPercent(uint8_t row, uint8_t column, uint8_t number) {
	LcdGoToPos(row, column);
	if (number < 100) {
		LcdSendData(' ');
	}
	if (number < 10) {
		LcdSendData(' ');
	}
	LcdConvertIntDisplay(number);
	LcdSendData('%');
}

/**
 * Метод отображает текущее значение давления в кПа.
 */
void showPressureStatus() {
	LcdGoToPos(1, 10);
	LcdFillSpace(6);
	LcdGoToPos(1, 10);
	LcdConvertIntDisplay(pressure);
	LcdDrawString("kPa ");
}

/**
 * Метод обновляет информацию о текущем состояни управляемого устройства, а так же управляет им.
 */
void updateDeviceStatus() {
	LcdGoToPos(0, 6);
	if (currentDeviceState == ON) {
		LcdDrawString("ON ");
		GPIO_SetBits(GPIOA, DEVICE_OUT_PA7);
		return;
	}
	LcdDrawString("OFF");
	GPIO_ResetBits(GPIOA, DEVICE_OUT_PA7);
}

/**
 * Метод обновляет значение переменной, создержащей значение давления в кПа,
 * а так же обновляет состояние управляемого устройства.
 */
void setPressure(uint16_t pressureValue) {
	if (pressureValue >= getMaxThreshold()) {
		currentDeviceState = OFF;
	} else if (pressureValue < getMinThreshold()) {
		currentDeviceState = ON;
	}
	pressure = pressureValue;
}

/**
 * Метод инициализации переменной, содержащей значение заряда батареи.
 */
void setCharge(uint8_t chargeValue) {
	charge = chargeValue;
}

/**
 * Метод инициализации переменной, содержащей значение текущего состояния управляемого устройства.
 */
void setCurrentDeviceState(uint8_t state) {
	currentDeviceState = state;
}
