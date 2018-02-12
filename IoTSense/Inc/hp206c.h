/*
 * hp206c.h
 *
 *  Created on: 04.02.2018
 *      Author: Laurent
 */

#ifndef HP206C_H_
#define HP206C_H_

void hp206c_init(void);
void hp206c_performConversion(void);
uint32_t hp206c_getTemperature(void);
uint32_t hp206c_getPressure(void);
uint32_t hp206c_getAltitude(void);

#endif /* HP206C_H_ */
