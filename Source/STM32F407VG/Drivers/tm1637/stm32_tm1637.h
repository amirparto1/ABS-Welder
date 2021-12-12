#pragma once

#include "stm32f4xx_hal.h"
void tm1637Init(void);
void tm1637Display(char c[]);
void tm1637SetBrightness(char brightness);
uint8_t tm1637ReadKey(void);
