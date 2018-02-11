/*
 * hp206c.c
 *
 *  Created on: 04.02.2018
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"

#define SLAVE_ADDRESS    0xEC

// Commands
#define ADC_CVT         0x40
#define DSR_1024        0x08
#define SOFT_RST        0x06
#define READ_PT         0x10
#define READ_P          0x30
#define READ_A          0x31
#define READ_T          0x32
#define READ_REG        0x80
#define WRITE_REG       0xC0

// Config registers
#define ALT_OFF_LSB     0x00
#define ALT_OFF_MSB     0x01
#define PA_H_TH_LSB     0x02
#define PA_H_TH_MSB     0x03
#define PA_M_TH_LSB     0x04
#define PA_M_TH_MSB     0x05
#define PA_L_TH_LSB     0x06
#define PA_L_TH_MSB     0x07
#define T_H_TH          0x08
#define T_M_TH          0x09
#define T_L_TH          0x0A
#define INT_EN          0x0B
#define INT_CFG         0x0C
#define INT_SRC         0x0D
#define INT_DIR         0x0E
#define PARA            0x0F

extern I2C_HandleTypeDef hi2c1;

static uint8_t hp206c_readRegister(uint8_t reg);
static void hp206c_writeRegister(uint8_t reg, uint8_t value);

void hp206c_init()
{
    uint8_t command;
    command = SOFT_RST;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, &command, 1, 100);
    HAL_Delay(10);

    hp206c_writeRegister(INT_DIR, 0x80);
    hp206c_writeRegister(PA_H_TH_MSB, 0xFF);
    hp206c_writeRegister(PA_M_TH_MSB, 0xA0);
    hp206c_writeRegister(PA_L_TH_MSB, 0x01);
    hp206c_writeRegister(T_H_TH, 0xFF);
    hp206c_writeRegister(T_M_TH, 0xA0);
    hp206c_writeRegister(T_L_TH, 0x01);
}

void hp206c_performConversion()
{
    uint8_t command = ADC_CVT | DSR_1024;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, &command, 1, 100);
    HAL_Delay(100);
}

uint32_t hp206c_getTemperature(void)
{
    uint32_t data = 0;
    uint8_t command = READ_T;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, &command, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS, &data, 3, 100);
    return data;

}

uint32_t hp206c_getPressure(void)
{
    uint32_t data = 0;
    uint8_t command = READ_P;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, &command, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS, &data, 3, 100);
    return data;
}

uint32_t hp206c_getAltitude(void)
{
    uint32_t data = 0;
    uint8_t command = READ_A;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, &command, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS, &data, 3, 100);
    return data;
}

static uint8_t hp206c_readRegister(uint8_t reg)
{
    uint8_t transmit_data = READ_REG | reg;
    uint8_t receive_data = 0;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, &transmit_data, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDRESS, &receive_data, 1, 100);
    return receive_data;
}

static void hp206c_writeRegister(uint8_t reg, uint8_t value)
{
    uint8_t command[2];
    command[0] = WRITE_REG | reg;
    command[1] = value;
    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS, command, 2, 100);
}

void hp206c_debug(uint8_t *buffer)
{
    buffer[0] = hp206c_readRegister(T_H_TH);
    buffer[1] = hp206c_readRegister(T_M_TH);
    buffer[2] = hp206c_readRegister(T_L_TH);
    buffer[3] = hp206c_readRegister(PA_H_TH_MSB);
    buffer[4] = hp206c_readRegister(PA_H_TH_LSB);
    buffer[5] = hp206c_readRegister(PA_M_TH_MSB);
    buffer[6] = hp206c_readRegister(PA_M_TH_LSB);
    buffer[7] = hp206c_readRegister(PA_L_TH_MSB);
    buffer[8] = hp206c_readRegister(PA_L_TH_LSB);
    buffer[9] = hp206c_readRegister(INT_EN);
    buffer[10] = hp206c_readRegister(INT_CFG);
    buffer[11] = hp206c_readRegister(INT_SRC);
    buffer[12] = hp206c_readRegister(PARA);
}
