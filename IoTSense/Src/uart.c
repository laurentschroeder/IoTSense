/*
 * uart.c
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */


#include "stm32f0xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "time.h"

#define COMMAND_MAX_SIZE 200

extern UART_HandleTypeDef huart1;
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

static void uart_parse_command(uint8_t command[])
{
    if(strcmp("get timestamp\r\n", (char *)command) == 0)
    {
        // call corresponding function from time.c
        char *msg = "\n\rSucces Laurent!\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);
    }
    else if(strcmp("2000\r\n", (char *)command) == 0)
    {
        // call corresponding function from time.c
        char *msg = "\n\rAgain succes Laurent!\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);
    }
    else if(strlen((char *)command) > 8)
    {
        //HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), 100);
        char str1[8];
        char str2[8];
        strncpy(str1, "set time", 8);
        strncpy(str2, (char *) command, 8);
        if(strncmp(str1, str2, 8) == 0)
        {
            set_time((char *)command);
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(i<COMMAND_MAX_SIZE)
        {
            command_buffer[i] = uart_rec_data;
            i++;
            if(uart_rec_data == '\n')
            {
                uint8_t cmd_len = strlen((char *)command_buffer) + 1;
                uint8_t command[cmd_len];
                strncpy((char *)command, (char *)command_buffer, cmd_len);
                uart_parse_command(command);
                i = 0;
                memset(command_buffer, 0, sizeof(command_buffer));
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
