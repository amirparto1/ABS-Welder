#pragma once

#include "stm32f0xx_hal.h"
typedef struct
{
	GPIO_TypeDef *CLK_GPIO;
	uint16_t CLK_Pin;
	GPIO_TypeDef *Data_GPIO;
	uint16_t Data_Pin;
}TM1637_structure;
void tm1637Init(TM1637_structure TM1637);
void tm1637Display(TM1637_structure TM1637 , char c[]);
void tm1637SetBrightness(TM1637_structure TM1637 , char brightness);
uint8_t tm1637ReadKey(TM1637_structure TM1637);
