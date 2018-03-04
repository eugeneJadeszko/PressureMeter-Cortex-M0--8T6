#include "utility.h"
#include <stdlib.h>

#define SYSCLK 48

/**
 * Задержка в мкс.
 */
void delay_us(uint32_t t)
{
	unsigned long i;
	i = t*SYSCLK;
	while(i--);
}

/**
 * Задержка в мс.
 */
void delay_ms(uint32_t t)
{
	unsigned long i;
	i = t*SYSCLK*100;
	while(i--);
}

uint8_t ConvertIntToString(int n, char* s)
{
	uint8_t size = 0;
    int i = 0, sign;
    uint8_t c[12];

     if ((sign = n) < 0)  n = -n;
     do
     {
         c[i++] = n % 10 + '0';   // берем следующую цифру
         size++;
     } while ((n /= 10) > 0);     // удаляем
     if (sign < 0)
     {
         c[i++] = '-';
         size++;
     }
     //c[i] = '\0';

     int x;
     for(x = 0;x < size;x++)
    	s[x] = c[size - x -1];

     return size;
}

uint8_t ConvertFloatToString(float f,char* s)
{
	uint8_t size = 0;
	char s1[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	char s2[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int32_t h = (int32_t)f;
	size = ConvertIntToString(h,s1);
	if(f < 0.f) f *= -1.f;
	if(h < 0)   h *= -1;
	int32_t l = (int32_t)((f - (float)h)*1000.f);
	size += ConvertIntToString(l,s2);

	uint8_t x = 0,y = 0;
	while(s1[x])
	{
		s[x] = s1[x];
		x++;
	}
	s[x] = '.';
	x++;
	while(s2[y])
	{
		s[x+y] = s2[y];
		y++;
	}

    return size+1;
}

int32_t ConvertStringToInt(char* s, uint8_t pos)
{
	uint8_t i = pos;
	uint8_t z = 0;
	char buf[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	while(s[i])
	{
		if(((s[i]>47) && (s[i]<58)) || (s[i]=='-'))
		{
			buf[z] = s[i];
		}
		else
		{
			break;
		}
		if(i < 63)	i++;
		else		break;
		if(z < 11)	z++;
		else		break;
	}
	return atoi(buf);
}

float ConvertStringToFloat(char* s, uint8_t pos)
{
	uint8_t i = pos;
	uint8_t z = 0;
	char buf[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	while(s[i])
	{
		if(((s[i]>47) && (s[i]<58)) || (s[i]=='-') || (s[i]=='.'))
		{
			buf[z] = s[i];
		}
		else
		{
			break;
		}
		if(i <63)	i++;
		else		break;
		if(z < 11)	z++;
		else		break;
		}

	return atof(buf);//1.0f;//strtod(buf,NULL);
}
