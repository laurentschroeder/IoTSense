/*
 * uart.c
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */


#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

void uart_send(uint8_t *buffer)
{
    HAL_UART_Transmit(&huart2, buffer, strlen((char *)buffer), 100);
}
