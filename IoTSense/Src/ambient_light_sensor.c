/*
 * ambient_light_sensor.c
 *
 *  Created on: 18.12.2017
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"
#include "ambient_light_sensor.h"

extern I2C_HandleTypeDef hi2c1;

static uint8_t ReadFromRegister(uint8_t reg)
{
    uint8_t message;
    HAL_I2C_Mem_Read(&hi2c1, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, &message, 1, 100);
    return message;
}

static void WriteToRegister(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(&hi2c1, SLAVE_ADDRESS<<1, reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 100);
}

static void ParamSet(uint8_t address, uint8_t value)
{
    WriteToRegister(PARAM_WR_REG, value);
    WriteToRegister(COMMAND_REG, PARAM_SET | address);
}

static uint8_t ParamGet(uint8_t address)
{
    WriteToRegister(COMMAND_REG, PARAM_QUERY | address);
    uint8_t message = ReadFromRegister(PARAM_RD_REG);
    return message;
}

void reset()
{
    WriteToRegister(COMMAND_REG, RESET);
}

void init()
{
    WriteToRegister(HW_KEY_REG, 0x17);
    HAL_Delay(150);
    ParamSet(ALS_VIS_ADC_COUNTER, ADC_Clock_127);
    ParamSet(ALS_VIS_ADC_GAIN, ADC_CLOCK_DIV128);
    ParamSet(ALS_VIS_ADC_MISC, VIS_RANGE_NORMAL);

}

uint16_t GetVisible()
{
    uint16_t data = 0;
    ParamSet(CHLIST, EN_ALS_VIS);
    WriteToRegister(COMMAND_REG, ALS_FORCE);
    HAL_Delay(150);
    uint8_t vis0 = ReadFromRegister(ALS_VIS_DATA0);
    uint8_t vis1 = ReadFromRegister(ALS_VIS_DATA1);
    data = vis0 + (vis1 << 8);
    return data;
}

uint8_t GetResponse()
{
    return ReadFromRegister(RESPONSE_REG);
}

void SendNOP()
{
    WriteToRegister(COMMAND_REG, 0x00);
}
