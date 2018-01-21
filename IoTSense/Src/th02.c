/*
 * th02.c
 *
 *  Created on: 21.01.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"

#define SLAVE_ADDRESS   0x40

#define STATUS_REG      0x00
#define DATAh_REG       0x01
#define DATAl_REG       0x02
#define CONFIG_REG      0x03

#define START           0x01
#define RDY             0x01
#define TEMP            0x10

extern I2C_HandleTypeDef hi2c1;

static uint8_t ReadFromRegister(uint8_t reg);
static void WriteToRegister(uint8_t reg, uint8_t value);
static uint16_t Read2ByteFromRegister(uint8_t reg);

static uint8_t ReadFromRegister(uint8_t reg)
{
    uint8_t message;
    HAL_I2C_Mem_Read(&hi2c1, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, &message, 1, 100);
    return message;
}

static uint16_t Read2ByteFromRegister(uint8_t reg)
{
    uint8_t message[2];
    HAL_I2C_Mem_Read(&hi2c1, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, message, 2, 100);
    uint16_t value = (message[0] << 8) + message[1];
    return value;
}

static void WriteToRegister(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(&hi2c1, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}

uint16_t th02_get_humidity()
{
    WriteToRegister(CONFIG_REG, START);
    while(ReadFromRegister(STATUS_REG) & RDY)
        ;
    uint16_t humidity = Read2ByteFromRegister(DATAh_REG);
    return humidity >> 4;
}

uint16_t th02_get_temperature()
{
    WriteToRegister(CONFIG_REG, START | TEMP);
    while(ReadFromRegister(STATUS_REG) & RDY)
            ;
    uint16_t temperature = Read2ByteFromRegister(DATAh_REG);
    return temperature >> 2;
}
