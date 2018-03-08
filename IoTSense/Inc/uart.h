/*
 * uart.h
 *
 *  Created on: 19.02.2018
 *      Author: Laurent
 */

#ifndef UART_H_
#define UART_H_

/**
 * @brief   send data over UART to the WiFly WLAN module
 *
 * @param   buffer
 *          String buffer castet as uint8_t for HAL compatibility
 */
void uart_send(uint8_t *buffer);

/**
 * @brief   Enable UART interrupt for incoming bytes
 *          Will be parsed internally by uart.c
 */
void uart_receive_enable(void);

#endif /* UART_H_ */
