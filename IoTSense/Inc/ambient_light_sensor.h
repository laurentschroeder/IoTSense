/*
 * ambient_light_sensor.h
 *
 *  Created on: 18.12.2017
 *      Author: Laurent
 */

#ifndef AMBIENT_LIGHT_SENSOR_H_
#define AMBIENT_LIGHT_SENSOR_H_

void si1145_init();
void si1145_reset();
uint16_t si1145_getVisible();
uint16_t si1145_getUVIndex();
uint8_t si1145_getResponse();
void si1145_sendNOP();
char* si1145_getStatus();


#endif /* AMBIENT_LIGHT_SENSOR_H_ */
