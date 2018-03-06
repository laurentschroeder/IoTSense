/*
 * time.c
 *
 *  Created on: 05.03.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"

uint8_t time_received = 0;

void wait_for_time(void)
{
    while(time_received != 1)
    {

    }
}

void unlock_time(void)
{
    time_received = 1;
}
