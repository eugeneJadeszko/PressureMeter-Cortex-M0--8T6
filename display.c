#include "display.h"
#include "hd44780.h"
#include "buttons.h"
#include "utility.h"
#include "timer.h"
#include "gpio.h"

uint8_t batEmpty[8] = { 14, 17, 17, 17, 17, 17, 17, 31 };
uint8_t bat10[8] = { 14, 17, 17, 17, 17, 17, 31, 31 };
uint8_t bat30[8] = { 14, 17, 17, 17, 17, 31, 31, 31 };
uint8_t bat50[8] = { 14, 17, 17, 17, 31, 31, 31, 31 };
uint8_t bat70[8] = { 14, 17, 17, 31, 31, 31, 31, 31 };
uint8_t bat95[8] = { 14, 17, 31, 31, 31, 31, 31, 31 };
uint8_t batFull[8] = { 14, 31, 31, 31, 31, 31, 31, 31 };
uint16_t maxThreshold = 600;
uint16_t minThreshold = 100;

//But MENU_OK_PA10 = { B_MENU_OK_PA10, NOT_PRESSED };
//But PLUS_PA9 = { B_PLUS_PA9, NOT_PRESSED };

void DisplayInit(void) {
	TIM_Cmd(TIM2, DISABLE);
	LcdInit();
	BatteryIconDraw();
	LcdGoToPos(0, 2);
	LcdDrawString("PromEl  2018");
	delay_ms(500);
	LcdGoToPos(1, 1);
	LcdDrawString("Pressure meter");
	delay_ms(500);
	TIM_Cmd(TIM2, ENABLE);
}

void Display(void) {
	LcdClear();
	delay_ms(2);
	mainDisplay();
//	while (button(MENU_OK_PA10) == NOT_PRESSED) {
//		}
	while (buttonMenuOk() == NOT_PRESSED) {
	}
	menu();
}

void mainDisplay() {
	LcdGoToPos(0, 0);
	LcdDrawString("State: ");
	LcdGoToPos(1, 0);
	LcdDrawString("Pressure: ");
}

void menu() {
	TIM_Cmd(TIM2, DISABLE);
	uint8_t menuItem = 0;
	while (menuItem != 4) {
		if (menuItem > 3) {
			menuItem = 0;
		}
		switch (menuItem) {
		case 0:
			itemMaxThreshold();
			menuItem++;
			break;
		case 1:
			itemMinThreshold();
			menuItem++;
			break;
		case 2:
			itemBrightnessControl();
			menuItem++;
			break;
		default:
			menuItem = quit();
			break;
		}
	}
	TIM_Cmd(TIM2, ENABLE);
}

void itemMaxThreshold() {
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("1.");
	LcdGoToPos(1, 0);
	LcdDrawString("Upper threshold");
	while (buttonPlus() == NOT_PRESSED) {
		if (buttonMenuOk() == PRESSED) {
			setMaxThreshold();
		}
	}
}

void itemMinThreshold() {
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("2.");
	LcdGoToPos(1, 0);
	LcdDrawString("Lower threshold");
	while (buttonPlus() == NOT_PRESSED) {
		if (buttonMenuOk() == PRESSED) {
			setMinThreshold();
		}
	}
}

void itemBrightnessControl() {
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("3.");
	LcdGoToPos(1, 0);
	LcdDrawString("Bright control");
	while (buttonPlus() == NOT_PRESSED) {
		if (buttonMenuOk() == PRESSED) {
			brightnessControl();
		}
	}
}

uint8_t quit() {
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("4.");
	LcdGoToPos(1, 0);
	LcdDrawString("Exit");
	while (buttonPlus() == NOT_PRESSED) {
		if (buttonMenuOk() == PRESSED) {
			return 4;
		}
	}
	return 0;
}

void brightnessControl() {
	int8_t step = 5;
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("Set...");
	LcdGoToPos(1, 0);
	LcdDrawString("Bright= ");
	LcdConvertIntDisplay(getLcdBrightness());
	LcdGoToPos(1, 11);
	LcdSendData('%');
	while (step != 0) {
		switch (step) {
		case 5:
			highLighting(&step, getLcdBrightness());
			break;
		case -5:
			dehighLighting(&step, getLcdBrightness());
			break;
		default:
			break;
		}
	}
	LcdGoToPos(0, 0);
	LcdDrawString("Saving...");
	delay_ms(500);
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("3.");
	LcdGoToPos(1, 0);
	LcdDrawString("Bright control");
}

void dehighLighting(int8_t *step, uint8_t lcdBrightness) {
	while (1) {
		if (buttonPlus() == PRESSED) {
			if (lcdBrightness > 5) {
				lcdBrightness += *step;
				LcdGoToPos(1, 8);
				LcdConvertIntDisplay(lcdBrightness);
				setLcdBrightness(lcdBrightness);
			} else if (lcdBrightness <= 5) {
				lcdBrightness = 0;
				LcdGoToPos(1, 8);
				LcdConvertIntDisplay(lcdBrightness);
				setLcdBrightness(lcdBrightness);
				*step = -*step;
				break;
			}
			if (lcdBrightness < 100) {
				LcdGoToPos(1, 10);
				LcdFillSpace(1);
			}
			if (lcdBrightness < 10) {
				LcdGoToPos(1, 9);
				LcdFillSpace(1);
			}
		}
		if (buttonMenuOk() == PRESSED) {
			*step = 0;
			break;
		}
	}
}

void highLighting(int8_t *step, uint8_t lcdBrightness) {
	while (1) {
		if (buttonPlus() == PRESSED) {
			if (lcdBrightness < 95) {
				lcdBrightness += *step;
				LcdGoToPos(1, 8);
				LcdConvertIntDisplay(lcdBrightness);
				setLcdBrightness(lcdBrightness);
			} else if (lcdBrightness >= 95) {
				lcdBrightness = 100;
				LcdGoToPos(1, 8);
				LcdConvertIntDisplay(lcdBrightness);
				setLcdBrightness(lcdBrightness);
				*step = -*step;
				break;
			}
		}
		if (buttonMenuOk() == PRESSED) {
			*step = 0;
			break;
		}
	}
}

void setMinThreshold() {
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("SetMin...");
	LcdGoToPos(1, 0);
	LcdDrawString("Threshold= ");
	LcdConvertIntDisplay(minThreshold);
	while (1) {
		if (buttonPlus() == PRESSED) {
			if (minThreshold < maxThreshold - 50) {
				minThreshold += 50;
			} else if (minThreshold <= maxThreshold - 50) {
				minThreshold = 100;
			}
			LcdGoToPos(1, 11);
			LcdConvertIntDisplay(minThreshold);
		}
		if (buttonMenuOk() == PRESSED) {
			LcdGoToPos(0, 0);
			LcdDrawString("Saving...");
			delay_ms(500);
			LcdGoToPos(0, 0);
			LcdDrawString("2.");
			LcdFillSpace(9);
			LcdGoToPos(1, 0);
			LcdDrawString("Lower threshold");
			break;
		}
	}
}

void setMaxThreshold() {
	LcdClear();
	delay_ms(2);
	LcdGoToPos(0, 0);
	LcdDrawString("SetMax...");
	LcdGoToPos(1, 0);
	LcdDrawString("Threshold= ");
	LcdConvertIntDisplay(maxThreshold);
	while (1) {
		if (buttonPlus() == PRESSED) {
			if (maxThreshold <= 550) {
				maxThreshold += 50;
			} else if (maxThreshold >= 600) {
				maxThreshold = minThreshold + 50;
			}
			LcdGoToPos(1, 11);
			LcdConvertIntDisplay(maxThreshold);
		}
		if (buttonMenuOk() == PRESSED) {
			LcdGoToPos(0, 0);
			LcdDrawString("Saving...");
			delay_ms(500);
			LcdGoToPos(0, 0);
			LcdDrawString("1.");
			LcdFillSpace(9);
			LcdGoToPos(1, 0);
			LcdDrawString("Upper threshold");
			break;
		}
	}
}

uint16_t getMaxThreshold() {
	return maxThreshold;
}

uint16_t getMinThreshold() {
	return minThreshold;
}

void IconDraw(uint8_t number, uint8_t * char_data) {
	uint8_t i;
	LcdSendCommand((1 << 6) | (number * 8));
	for (i = 0; i < 8; ++i) {
		LcdSendData(char_data[i]);
	}
	LcdSendCommand(1 << 7);
}

void BatteryIconDraw(void) {
	IconDraw(0, batEmpty);
	IconDraw(1, bat10);
	IconDraw(2, bat30);
	IconDraw(3, bat50);
	IconDraw(4, bat70);
	IconDraw(5, bat95);
	IconDraw(6, batFull);
}
