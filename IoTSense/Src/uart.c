/*
 * uart.c
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */


#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <string.h>

#define COMMAND_MAX_SIZE 200

extern UART_HandleTypeDef huart2;
uint8_t command_buffer[COMMAND_MAX_SIZE];
uint8_t uart_rec_data;
uint8_t i = 0;

void uart_send(uint8_t *buffer)
{
    HAL_UART_Transmit(&huart2, buffer, strlen((char *)buffer), 100);
    HAL_Delay(500);
}

void uart_receive_enable(void)
{
    HAL_UART_Receive_IT(&huart2, &uart_rec_data, 1);
}

static void uart_parse_command()
{
    if(strcmp("get timestamp\r\n", (char *)command_buffer) == 0)
    {
        // call corresponding function from time.c
        HAL_UART_Transmit(&huart2, (uint8_t *)"Succes Laurent!\r\n", strlen("Succes Laurent!\r\n"), 100);
    }
    memset(command_buffer, 0, sizeof(command_buffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(i<COMMAND_MAX_SIZE)
    {
        command_buffer[i] = uart_rec_data;
        i++;
        if(uart_rec_data == '\n')
        {
            uart_parse_command();
            i = 0;
        }
    }
    else
    {
        char *error_msg = "Message too long!\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)error_msg, strlen(error_msg), 100);
        memset(command_buffer, 0, sizeof(command_buffer));
        i = 0;
    }
    HAL_UART_Receive_IT(&huart2, &uart_rec_data, 1);
}
