#include "hd44780.h"
#include "utility.h"
#include "gpio.h"
#include "timer.h"

static uint8_t u8_cur_pos;

/**
 * Метод инициализации дисплея и перевода его в 4-битный режим.
 */
void LcdInit(void) {
	u8_cur_pos = 0;

	RCC_AHBPeriphClockCmd(RCC_LCD_DAT_PORT, ENABLE);
	GPIO_InitStructure.GPIO_Pin = DB4 | DB5 | DB6 | DB7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(LCD_DAT_PORT, &GPIO_InitStructure);

	RCC_AHBPeriphClockCmd(RCC_LCD_CONT_PORT, ENABLE);
	GPIO_InitStructure.GPIO_Pin = (RS | EN);
	GPIO_Init(LCD_CONT_PORT, &GPIO_InitStructure);

	delay_ms(200);
	LcdSendCommand(0x33);
	delay_ms(70);
	LcdSendCommand(0x32);
	delay_us(100);
	LcdSendCommand(0x28);
	LcdSendCommand(0x08);
	LcdSendCommand(0x01);
	delay_ms(200);
	LcdSendCommand(0x06);
	LcdSendCommand(0x0C);
}

/**
 * Метод включения подсветки дисплея.
 */
void LcdLedOn(void) {
	setBrightness(getLcdBrightness());
	TIM_Cmd(TIM1, ENABLE);
}

/**
 * Метод отключения подсветки дисплея.
 */
void LcdLedOff(void) {
	setBrightness(0);
	TIM_Cmd(TIM1, DISABLE);
}

/**
 * Метод задающий режим курсора.
 * curState - включение/отключение курсора.
 * curMode  - режим курсора: мигающий/статический.
 */
void LcdCursorMode(cursor_state curState, cursor_mode curMode) {
	if (curState == CURSOR_DISABLE) {
		if (curMode == NO_BLINK) {
			LcdSendCommand(0x0C);
		} else {
			LcdSendCommand(0x0D);
		}
	} else {
		if (curMode == NO_BLINK) {
			LcdSendCommand(14);
		} else {
			LcdSendCommand(15);
		}
	}
}

/**
 * Метод отправки команды для дисплея.
 */
void LcdSendCommand(uint8_t data) {
	GPIO_ResetBits(LCD_CONT_PORT, RS);
	LcdSendByte(data);
}

/**
 * Метод отправки данных для отображения на дисплей.
 */
void LcdSendData(uint8_t data) {
	GPIO_SetBits(LCD_CONT_PORT, RS);
	LcdSendByte(data);
}

void LcdSendByte(uint8_t data) {
	GPIO_SetBits(LCD_CONT_PORT, EN);
	if (data & 16)
		GPIO_SetBits(LCD_DAT_PORT, DB4);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB4);
	if (data & 32)
		GPIO_SetBits(LCD_DAT_PORT, DB5);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB5);
	if (data & 64)
		GPIO_SetBits(LCD_DAT_PORT, DB6);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB6);
	if (data & 128)
		GPIO_SetBits(LCD_DAT_PORT, DB7);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB7);
	__NOP();
	GPIO_ResetBits(LCD_CONT_PORT, EN);
	delay_us(40);
	GPIO_SetBits(LCD_CONT_PORT, EN);
	if (data & 1)
		GPIO_SetBits(LCD_DAT_PORT, DB4);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB4);
	if (data & 2)
		GPIO_SetBits(LCD_DAT_PORT, DB5);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB5);
	if (data & 4)
		GPIO_SetBits(LCD_DAT_PORT, DB6);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB6);
	if (data & 8)
		GPIO_SetBits(LCD_DAT_PORT, DB7);
	else
		GPIO_ResetBits(LCD_DAT_PORT, DB7);
	__NOP();
	GPIO_ResetBits(LCD_CONT_PORT, EN);
	delay_us(40);
	u8_cur_pos++;
}

/**
 * Метод, отображающий строку на дисплее.
 * text - указатель на массив типа char.
 */
uint8_t LcdDrawString(char* text) {
	uint8_t size = 0;
	while (*text) {
		LcdSendData(*text++);
		size++;
	}
	return size;
}

/**
 * Установка курсора на позицию.
 * row - номер строки.
 * col - номер столбца.
 */
void LcdGoToPos(char row, char col) {
	char address;
	if (row == 0)
		address = 0;
	else
		address = 0x40;
	address |= col;
	LcdSendCommand(0x80 | address);
	u8_cur_pos = address;
}

/**
 * Очистка дисплея.
 */
void LcdClear(void) {
	LcdSendCommand(0x01);
	delay_ms(2);
	u8_cur_pos = 0;
}

/**
 * Отображение 32-битного числа на дисплее.
 */
uint8_t LcdConvertIntDisplay(int32_t u) {
	uint8_t n = 0;
	uint8_t c[12] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 };
	itoa(u, c);
	uint8_t i;
	for (i = 0; i < 12; i++)
		if ((c[i] != 0))
			if ((c[i] != 32)) {
				LcdSendData(c[i]);
				n++;
			}
	return n;
}

/**
 * Заполнение пробелами указанного количества позиций.
 * num - количество позиций, от текущего положения курсора, для заполнения пробелами.
 */
void LcdFillSpace(uint8_t num) {
	while (num--)
		LcdSendData(' ');
}

uint8_t LcdGetCurPos() {
	return u8_cur_pos;
}

uint8_t itoa(int n, uint8_t* s) {
	uint8_t size = 0;
	int i, sign;
	uint8_t c[12];

	if ((sign = n) < 0) // записываем знак
		n = -n; // делаем n положительным числом
	i = 0;
	do { // генерируем цифры в обратном пор¤дке
		c[i++] = n % 10 + '0'; // берем следующую цифру
		size++;
	} while ((n /= 10) > 0); // удал¤ем
	if (sign < 0) {
		c[i++] = '-';
		size++;
	}
	//c[i] = '\0';

	int x;
	for (x = 0; x < size; x++)
		s[x] = c[size - x - 1];

	return size;
}
