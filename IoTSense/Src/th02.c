/*
 * th02.c
 *
 *  Created on: 21.01.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"
#include "th02.h"

#define SLAVE_ADDRESS   0x40

#define STATUS_REG      0x00
#define DATAh_REG       0x01
#define DATAl_REG       0x02
#define CONFIG_REG      0x03

#define START           0x01
#define RDY             0x01
#define TEMP            0x10

extern I2C_HandleTypeDef hi2c2;

static uint8_t ReadFromRegister(uint8_t reg);
static void WriteToRegister(uint8_t reg, uint8_t value);
static uint16_t Read2ByteFromRegister(uint8_t reg);

static uint8_t ReadFromRegister(uint8_t reg)
{
    uint8_t message;
    HAL_I2C_Mem_Read(&hi2c2, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, &message, 1, 10);
    return message;
}

static uint16_t Read2ByteFromRegister(uint8_t reg)
{
    uint8_t message[2] = {0};
    HAL_I2C_Mem_Read(&hi2c2, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, message, 2, 10);
    uint16_t value = (message[0] << 8) + message[1];
    return value;
}

static void WriteToRegister(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(&hi2c2, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
}

uint16_t th02_get_humidity_raw()
{
    WriteToRegister(CONFIG_REG, START);
    while(ReadFromRegister(STATUS_REG) & RDY)
        HAL_Delay(10);

    uint16_t humidity = Read2ByteFromRegister(DATAh_REG);
    return humidity >> 4;
}

uint16_t th02_get_temperature_raw()
{
    WriteToRegister(CONFIG_REG, START | TEMP);
    while(ReadFromRegister(STATUS_REG) & RDY)
        HAL_Delay(10);
    uint16_t temperature = Read2ByteFromRegister(DATAh_REG);
    return temperature >> 2;
}

float th02_get_humidity()
{
    float humidity_raw = th02_get_humidity_raw();
    humidity_raw /= 16;
    humidity_raw -= 24;

    // Linearization
    float a0 = -4.7844;
    float a1 = 0.4008;
    float a2 = -0.00393;
    float humidity_linear = humidity_raw - ((humidity_raw * humidity_raw) * a2 + humidity_raw * a1 + a0);

    // Temperature compensation
    float q0 = 0.1973;
    float q1 = 0.00237;

    float temperature = th02_get_temperature();
    float humidity_compensated = humidity_linear + ((temperature - 30)) * (humidity_linear * q1 + q0);

    return humidity_compensated;
}

float th02_get_temperature()
{
    float temperature = th02_get_temperature_raw();
    temperature /= 32;
    temperature -= 50;
    return temperature;
}
