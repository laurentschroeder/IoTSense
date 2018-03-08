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
    char timestamp[20];
} json_obj;

/**
 * @brief create json_object struct instance
 * @param json_object
 * @param id
 * @param sub_id
 * @param name
 * @param measure_type
 * @param unit
 * @param room
 */
void create_json_object(json_obj *json_object, uint8_t id, uint8_t sub_id,
        char name[], char measure_type[], char unit[],
        char room[]);

/**
 * @brief transform json_obj values to json string
 * @param sensor
 *          the json_obj to transform
 *
 * @param buffer
 *          String to store transformed values
 */
void create_json_string(json_obj sensor, uint8_t *buffer);


#endif /* JSON_H_ */
