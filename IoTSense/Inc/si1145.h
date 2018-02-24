/*
 * ambient_light_sensor.h
 *
 *  Created on: 18.12.2017
 *      Author: Laurent
 */

#ifndef SI1145_H_
#define SI1145_H_

#include "stm32f0xx_hal.h"

enum E_Operation {INDOOR_BULB, INDOOR_FLUORESCENT, OUTDOOR, HIGH_SENSITIVE};

void si1145_init(enum E_Operation operation);
float si1145_getVisible(void);
float si1145_getIR(void);
uint16_t si1145_getUVIndex(void);

#endif /* SI1145_H_ */
