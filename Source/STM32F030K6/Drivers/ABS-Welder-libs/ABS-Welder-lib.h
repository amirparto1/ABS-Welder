/*
 * ABS-Welder-lib.h
 *
 *  Created on: Feb 18, 2022
 *      Author: Amir
 */

#ifndef ABS_WELDER_LIBS_ABS_WELDER_LIB_H_
#define ABS_WELDER_LIBS_ABS_WELDER_LIB_H_

#include "stm32f0xx.h"

#define SERIESRESISTOR 			10000
#define NOMINAL_RESISTANCE 		100000
#define NOMINAL_TEMPERATURE 	25
#define BCOEFFICIENT 			3950
#define NTC_ADC_Handle 			hadc


float NTC_Read_Temperature(uint8_t average_count);

#endif /* ABS_WELDER_LIBS_ABS_WELDER_LIB_H_ */
