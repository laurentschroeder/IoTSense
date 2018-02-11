/*
 * ambient_light_sensor.h
 *
 *  Created on: 18.12.2017
 *      Author: Laurent
 */

#ifndef SI1145_H_
#define SI1145_H_

#include "stm32f0xx_hal.h"

void si1145_init();
uint32_t si1145_getVisible();
uint32_t si1145_getIR();
uint32_t si1145_getUVIndex();
uint8_t si1145_getResponse();
void si1145_sendNOP();
char* si1145_getStatus();



#endif /* SI1145_H_ */
