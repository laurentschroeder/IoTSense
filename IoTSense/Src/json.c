/*
 * json.c
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */


#include "json.h"
#include <stdio.h>
#include <string.h>

void create_json(sensor_type sensor, uint8_t *buffer)
{
    sprintf((char *)buffer, "{Sensor ID:%u,"
            "Sensor Sub ID:%u,"
            "Sensor:%s,"
            "Art der Messung:%s,"
            "Messwert:%ld,"
            "Einheit:%s,"
            "Raum:%s,"
            "Zeitstempel:%ld}",
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
