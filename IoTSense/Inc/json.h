/*
 * json.h
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */

#ifndef JSON_H_
#define JSON_H_

#include "stm32f0xx_hal.h"

typedef struct {
    uint8_t id;
    uint8_t sub_id;
    char name[20];
    char measure_type[20];
    uint32_t value;
    char unit[10];
    char room[20];
    uint32_t timestamp;
} sensor_type;

void create_json(sensor_type sensor, uint8_t *buffer);

#endif /* JSON_H_ */
