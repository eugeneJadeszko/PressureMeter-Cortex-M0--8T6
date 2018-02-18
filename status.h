#ifndef _STATUS_H_
#define _STATUS_H_

#include "inttypes.h"

typedef enum {
	CHARGE,
	STANDBY = 100,
	DISCHARGE,
	PERCENT_ROW = 0,
	PERCENT_COL = 11,
	ICON_ROW = 0,
	ICON_COL = 15
} Battery;

void updateStatuses();
void showBatteryStatus();
uint8_t getBatteryState();
void showPressureStatus();
void updateDeviceStatus();

void showChargeAsIcon(uint8_t row, uint8_t column, uint8_t chargeValue);
void showNumberAsPercent(uint8_t row, uint8_t column, uint8_t number);

void setPressure(uint16_t pressureValue);
void setCharge(uint8_t chargeValue);
void setCurrentDeviceState(uint8_t state);

#endif
