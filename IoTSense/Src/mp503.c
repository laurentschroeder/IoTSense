/*
 * mp503.c
 *
 *  Created on: 12.03.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"

extern ADC_HandleTypeDef hadc;

static const float adc_voltage_ratio = 0.00322265625;

float mp503_get_value(void)
{
    float value = 0;
    HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 100);
    value = HAL_ADC_GetValue(&hadc);
    HAL_ADC_Stop(&hadc);
    return value * adc_voltage_ratio;
}

