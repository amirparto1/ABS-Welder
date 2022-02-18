/*
 * NTC.c
 *
 *  Created on: Feb 18, 2022
 *      Author: Amir
 */

#include "NTC.h"

extern ADC_HandleTypeDef NTC_ADC_Handle;

float NTC_Read_Temperature(uint8_t average_count)
{
	double ADCvalue, Resistance, steinhart, adc_temp;

	for(int var = 0 ; var < average_count ; ++var)
	{
		HAL_ADC_Start(&NTC_ADC_Handle);
		HAL_Delay(5);
		adc_temp = HAL_ADC_GetValue(&NTC_ADC_Handle);
		ADCvalue += adc_temp;
	}
	ADCvalue /= average_count;

//convert value to resistance
	Resistance = (4096 / ADCvalue) - 1;
	Resistance = SERIESRESISTOR / Resistance;

	steinhart = Resistance / NOMINAL_RESISTANCE;// (R/Ro)
	steinhart = log(steinhart);// ln(R/Ro)
	steinhart /= BCOEFFICIENT;// 1/B * ln(R/Ro)
	steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);// + (1/To)
	steinhart = 1.0 / steinhart;// Invert
	steinhart -= 273.15;// convert to C

	return steinhart;
}
