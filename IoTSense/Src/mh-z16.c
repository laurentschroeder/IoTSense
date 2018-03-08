/*
 * mh-z16.c
 *
 *  Created on: 08.03.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"

extern UART_HandleTypeDef huart1;
uint8_t uart_response[9];

uint16_t mhz16_get_value()
{
    uint8_t uart_request[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    HAL_UART_Transmit(&huart1, uart_request, 9, 100);
    HAL_UART_Receive(&huart1, uart_response, 9, 100);
    uint16_t gas_concentration = (uart_response[2]<<8) + uart_response[3];
    return gas_concentration;
}
