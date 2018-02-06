/*
 * hp206c.h
 *
 *  Created on: 04.02.2018
 *      Author: Laurent
 */

#ifndef HP206C_H_
#define HP206C_H_

void hp206c_init();
void hp206c_performConversion();
void hp206c_getTemperatureAndPressure(uint8_t *buffer);
void hp206c_getAltitude(uint8_t *buffer);

#endif /* HP206C_H_ */
