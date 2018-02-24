/*
 * json.c
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */


#include "json.h"
#include <stdio.h>
#include <string.h>
#include "stm32f0xx_hal.h"

void create_json_string(json_obj sensor, uint8_t *buffer)
{
    sprintf((char *)buffer, "{\"Sensor ID\":%u,"
            "\"Sensor Sub ID\":%u,"
            "\"Sensor\":\"%s\","
            "\"Art der Messung\":\"%s\","
            "\"Messwert\":%ld,"
            "\"Einheit\":\"%s\","
            "\"Raum\":\"%s\","
            "\"Zeitstempel\":%ld}",
            sensor.id,
            sensor.sub_id,
            sensor.name,
            sensor.measure_type,
            sensor.value,
            sensor.unit,
            sensor.room,
            sensor.timestamp
            );
}

void create_json_object(json_obj *json_object, uint8_t id, uint8_t sub_id,
        char name[], char measure_type[], char unit[],
        char room[])
{
    json_object->id = id;
    json_object->sub_id = sub_id;
    strcpy(json_object->name, name);
    strcpy(json_object->measure_type, measure_type);
    json_object->value = 0;
    strcpy(json_object->unit, unit);
    strcpy(json_object->room, room);
    json_object->timestamp = HAL_GetTick();
}
