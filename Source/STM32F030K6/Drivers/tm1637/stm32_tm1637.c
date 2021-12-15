#include "../tm1637/stm32_tm1637.h"

#include <stdio.h>

// Configuration.
#define SegmentNo 4

void _tm1637Start(TM1637_structure TM1637);
void _tm1637Stop(TM1637_structure TM1637);
void _tm1637ReadResult(TM1637_structure TM1637);
void _tm1637WriteByte(TM1637_structure TM1637 , unsigned char b);
void _tm1637DelayUsec(unsigned int i);
void _tm1637ClkHigh(TM1637_structure TM1637);
void _tm1637ClkLow(TM1637_structure TM1637);
void _tm1637DioHigh(TM1637_structure TM1637);
void _tm1637DioLow(TM1637_structure TM1637);
uint8_t _tm1637Convert(char);

void tm1637Init(TM1637_structure TM1637)
{
	GPIO_InitTypeDef g = { 0 };
	g.Pull = GPIO_PULLUP;
	g.Mode = GPIO_MODE_OUTPUT_OD;// OD = open drain
	g.Speed = GPIO_SPEED_FREQ_HIGH;
	g.Pin = TM1637.CLK_Pin;
	HAL_GPIO_Init(TM1637.CLK_GPIO , &g);
	g.Pin = TM1637.Data_Pin;
	HAL_GPIO_Init(TM1637.Data_GPIO , &g);
}

uint8_t _tm1637Convert(char l)
{
	switch(l)
	{
		case '0':
			return 0x3f;
		case '1':
			return 0x06;
		case '2':
			return 0x5b;
		case '3':
			return 0x4f;
		case '4':
			return 0x66;
		case '5':
			return 0x6d;
		case '6':
			return 0x7d;
		case '7':
			return 0x07;
		case '8':
			return 0x7f;
		case '9':
			return 0x6f;
		case 'A':
			return 0x77;
		case 'a':
			return 0x77;
		case 'B':
			return 0x7C;
		case 'b':
			return 0x7C;
		case 'C':
			return 0x39;
		case 'c':
			return 0x58;
		case 'D':
			return 0x5E;
		case 'd':
			return 0x5E;
		case 'E':
			return 0x79;
		case 'e':
			return 0x79;
		case 'F':
			return 0x71;
		case 'f':
			return 0x71;
		case 'G':
			return 0x3d;
		case 'g':
			return 0x3d;
		case 'H':
			return 0x76;
		case 'h':
			return 0x74;
		case 'I':
			return 0x30;
		case 'i':
			return 0x30;
		case 'J':
			return 0x1E;
		case 'j':
			return 0x1E;
		case 'L':
			return 0x38;
		case 'l':
			return 0x38;
		case 'M':
			return 0x55;
		case 'm':
			return 0x55;
		case 'N':
			return 0x37;
		case 'n':
			return 0x54;
		case 'O':
			return 0x3F;
		case 'o':
			return 0x5C;
		case 'P':
			return 0x73;
		case 'p':
			return 0x73;
		case 'Q':
			return 0x67;
		case 'q':
			return 0x67;
		case 'R':
			return 0x50;
		case 'r':
			return 0x50;
		case 'S':
			return 0x6D;
		case 's':
			return 0x6D;
		case 'T':
			return 0x78;
		case 't':
			return 0x78;
		case 'U':
			return 0x3E;
		case 'u':
			return 0x1C;
		case 'W':
			return 0x6a;
		case 'w':
			return 0x6a;
		case 'Y':
			return 0x6E;
		case 'y':
			return 0x6E;
		default:
			return 0x00;
	}
}
void tm1637Display(TM1637_structure TM1637 , char c[])
{
	uint8_t digitArr[SegmentNo];
	int mc = 0;
	for(int jt = 0 ; (jt - mc) < SegmentNo ; jt++)
	{
		if(c[jt + mc] == '.')
		{
			digitArr[jt - 1] |= 0x80;
			mc++;
		}
		if(c[jt + mc])
			digitArr[jt] = _tm1637Convert(c[jt + mc]);
		else digitArr[jt] = 0x00;
	}

	_tm1637Start(TM1637);
	_tm1637WriteByte(TM1637 , 0x40);
	_tm1637ReadResult(TM1637);
	_tm1637Stop(TM1637);
	_tm1637Start(TM1637);
	_tm1637WriteByte(TM1637 , 0xc0);
	_tm1637ReadResult(TM1637);
	for(int it = 0 ; it < SegmentNo ; it++)
	{
		_tm1637WriteByte(TM1637 , digitArr[it]);
		_tm1637ReadResult(TM1637);
	}
	_tm1637Stop(TM1637);
}

uint8_t tm1637ReadKey(TM1637_structure TM1637)
{
	uint8_t retval = 0;

	_tm1637Start(TM1637);
	_tm1637WriteByte(TM1637 , 0x42);
	_tm1637ReadResult(TM1637);

	_tm1637DioHigh(TM1637);
	_tm1637DelayUsec(1);
	for(uint8_t bit = 0 ; bit < 8 ; bit++)
	{
		_tm1637ClkHigh(TM1637);
		retval <<= 1;
		if(HAL_GPIO_ReadPin(TM1637.Data_GPIO , TM1637.Data_Pin) == GPIO_PIN_SET)
		{
			retval |= 0x01;
		}
		_tm1637ClkLow(TM1637);
		_tm1637DelayUsec(1);

	}
	_tm1637DioLow(TM1637);

	_tm1637Stop(TM1637);

	return retval;

}

// Valid brightness values: 0 - 8.
void tm1637SetBrightness(TM1637_structure TM1637 , char brightness)
{
	_tm1637Start(TM1637);
	_tm1637WriteByte(TM1637 , 0x87 + brightness);
	_tm1637ReadResult(TM1637);
	_tm1637Stop(TM1637);
}

void _tm1637Start(TM1637_structure TM1637)
{
	_tm1637ClkHigh(TM1637);
	_tm1637DioHigh(TM1637);
	_tm1637DelayUsec(2);
	_tm1637DioLow(TM1637);
}

void _tm1637Stop(TM1637_structure TM1637)
{
	_tm1637ClkLow(TM1637);
	_tm1637DelayUsec(2);
	_tm1637DioLow(TM1637);
	_tm1637DelayUsec(2);
	_tm1637ClkHigh(TM1637);
	_tm1637DelayUsec(2);
	_tm1637DioHigh(TM1637);
}

void _tm1637ReadResult(TM1637_structure TM1637)
{
	_tm1637ClkLow(TM1637);
	_tm1637DelayUsec(5);
//while (HAL_GPIO_ReadPin(DIO_PORT,DIO_PIN)==1);
	_tm1637ClkHigh(TM1637);
	_tm1637DelayUsec(2);
	_tm1637ClkLow(TM1637);
}

void _tm1637WriteByte(TM1637_structure TM1637 , unsigned char b)
{
	for(int i = 0 ; i < 8 ; ++i)
	{
		_tm1637ClkLow(TM1637);
		if(b & 0x01)
		{
			_tm1637DioHigh(TM1637);
		}
		else
		{
			_tm1637DioLow(TM1637);
		}
		_tm1637DelayUsec(2);
		b >>= 1;
		_tm1637ClkHigh(TM1637);
		_tm1637DelayUsec(2);
	}
}

void _tm1637DelayUsec(unsigned int i)
{
	for(; i > 0 ; i--)
	{
		for(int j = 0 ; j < 60 ; ++j)
		{
			__NOP();
		}
	}
}

void _tm1637ClkHigh(TM1637_structure TM1637)
{
	HAL_GPIO_WritePin(TM1637.CLK_GPIO , TM1637.CLK_Pin , GPIO_PIN_SET);
}

void _tm1637ClkLow(TM1637_structure TM1637)
{
	HAL_GPIO_WritePin(TM1637.CLK_GPIO , TM1637.CLK_Pin , GPIO_PIN_RESET);
}

void _tm1637DioHigh(TM1637_structure TM1637)
{
	HAL_GPIO_WritePin(TM1637.Data_GPIO , TM1637.Data_Pin , GPIO_PIN_SET);
}

void _tm1637DioLow(TM1637_structure TM1637)
{
	HAL_GPIO_WritePin(TM1637.Data_GPIO , TM1637.Data_Pin , GPIO_PIN_RESET);
}
