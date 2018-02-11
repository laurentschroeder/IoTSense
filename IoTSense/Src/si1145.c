/*
 * ambient_light_sensor.c
 *
 *  Created on: 18.12.2017
 *      Author: Laurent
 */

#include "stm32f0xx_hal.h"

#define SLAVE_ADDRESS 0x60
#define ADC_OFFSET 255

// registers
#define SEQ_ID          0x02
#define HW_KEY          0x07
#define MEAS_RATE0      0x08
#define MEAS_RATE1      0x09
#define PS_LED21        0x0F
#define PS_LED3         0x10
#define UCOEF0          0x13
#define UCOEF1          0x14
#define UCOEF2          0x15
#define UCOEF3          0x16
#define PARAM_WR        0x17
#define COMMAND         0x18
#define RESPONSE        0x20
#define ALS_VIS_DATA0   0x22
#define ALS_VIS_DATA1   0x23
#define ALS_IR_DATA0    0x24
#define ALS_IR_DATA1    0x25
#define PS1_DATA0       0x26
#define PS1_DATA1       0x27
#define UVINDEX0        0x2C
#define UVINDEX1        0x2D
#define PARAM_RD        0x2E
#define CHIP_STAT       0x30

// commands
#define PARAM_QUERY     0x80
#define PARAM_SET       0xA0
#define RESET           0x01
#define ALS_FORCE       0x06
#define PSALS_FORCE     0x07
#define GET_CAL         0x12
#define NOP             0x00
#define PS_FORCE        0x05

// RAM address and parameters
#define CHLIST          0x01
#define EN_UV           0x80
#define EN_AUX          0x40
#define EN_ALS_IR       0x20
#define EN_ALS_VIS      0x10
#define EN_PS1          0x01

#define PSLED12_SELECT  0x02
#define LED1_ENABLE     0x01
#define LED1_OFF        0x00

#define PS_ADC_COUNTER  0x0A
#define PS_ADC_Clock_1     0x00    //50 ns times 2^PS_ADC_GAIN
#define PS_ADC_Clock_7     0x10    //350 ns times 2^PS_ADC_GAIN
#define PS_ADC_Clock_15    0x20    //750 ns times 2^PS_ADC_GAIN
#define PS_ADC_Clock_31    0x30    //1.55 탎 times 2^PS_ADC_GAIN
#define PS_ADC_Clock_63    0x40    //3.15 탎 times 2^PS_ADC_GAIN
#define PS_ADC_Clock_127   0x50    //6.35 탎 times 2^PS_ADC_GAIN
#define PS_ADC_Clock_255   0x60    //12.75 탎 times 2^PS_ADC_GAIN
#define PS_ADC_Clock_511   0x70    //25.55 탎 times 2^PS_ADC_GAIN

#define PS_ADC_GAIN     0x0B
#define PS_ADC_CLOCK_DIV1   0x00
#define PS_ADC_CLOCK_DIV16  0x04
#define PS_ADC_CLOCK_DIV32  0x05

#define PS_ADC_MISC     0x0C
#define PS_RANGE_NORMAL 0<<5
#define PS_RANGE_HIGH   1<<5
#define PS_ADC_MODE_RAW 0<<2
#define PS_ADC_MODE_PS  1<<2

#define ALS_IR_ADCMUX   0x0E
#define SMALL_IR        0x00
#define LARGE_IR        0x03

#define ALS_VIS_ADC_COUNTER 0x10
#define VIS_ADC_Clock_1    0x00    //50 ns times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_7    0x10    //350 ns times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_15   0x20    //750 ns times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_31   0x30    //1.55 ms times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_63   0x40    //3.15 탎 times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_127  0x50    //6.35 탎 times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_255  0x60    //12.75 탎 times 2^ALS_VIS_ADC_GAIN
#define VIS_ADC_Clock_511  0x70    //25.55 탎 times 2^ALS_VIS_ADC_GAIN

#define ALS_VIS_ADC_GAIN    0x11
#define VIS_ADC_CLOCK_DIV1      0x00
#define VIS_ADC_CLOCK_DIV2      0x01
#define VIS_ADC_CLOCK_DIV16     0x04
#define VIS_ADC_CLOCK_DIV64     0x06
#define VIS_ADC_CLOCK_DIV128    0x07

#define ALS_VIS_ADC_MISC    0x12
#define VIS_RANGE_NORMAL    0x00
#define VIS_RANGE_HIGH      0x20

#define ALS_IR_ADC_MISC     0x1F
#define IR_RANGE_NORMAL     0x0F
#define IR_RANGE_HIGH       0x1F

#define ALS_IR_ADC_COUNTER  0x1D
#define IR_ADC_Clock_1      0x00    //50 ns times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_7      0x10    //350 ns times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_15     0x20    //750 ns times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_31     0x30    //1.55 탎 times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_63     0x40    //3.15 탎 times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_127    0x50    //6.35 탎 times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_255    0x60    //12.75 탎 times 2^ALS_IR_ADC_GAIN
#define IR_ADC_Clock_511    0x70    //25.55 탎 times 2^ALS_IR_ADC_GAIN

#define ALS_IR_ADC_GAIN        0x1E
#define IR_ADC_CLOCK_DIV1      0x00
#define IR_ADC_CLOCK_DIV2      0x01
#define IR_ADC_CLOCK_DIV16     0x04
#define IR_ADC_CLOCK_DIV64     0x06
#define IR_ADC_CLOCK_DIV128    0x07

extern I2C_HandleTypeDef hi2c1;

static uint8_t ReadFromRegister(uint8_t reg);
static void WriteToRegister(uint8_t reg, uint8_t value);
static void ParamSet(uint8_t address, uint8_t value);
static uint8_t ParamGet(uint8_t address);
static void si1145_force();

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
    WriteToRegister(PARAM_WR, value);
    WriteToRegister(COMMAND, PARAM_SET | address);
}

static uint8_t ParamGet(uint8_t address)
{
    WriteToRegister(COMMAND, PARAM_QUERY | address);
    uint8_t message = ReadFromRegister(PARAM_RD);
    return message;
}

void si1145_init()
{
    WriteToRegister(COMMAND, RESET);
    HAL_Delay(10);

    WriteToRegister(HW_KEY, 0x17);
    HAL_Delay(10);

    ParamSet(ALS_VIS_ADC_COUNTER, VIS_ADC_Clock_255);
    ParamSet(ALS_VIS_ADC_GAIN, VIS_ADC_CLOCK_DIV2);
    ParamSet(ALS_VIS_ADC_MISC, VIS_RANGE_NORMAL);

    ParamSet(ALS_IR_ADC_COUNTER, IR_ADC_Clock_255);
    ParamSet(ALS_IR_ADC_GAIN, IR_ADC_CLOCK_DIV2);

    /*
     * Set IR RANGE in bit-field 5
     * bits 4:0 need to be preserved
     */
    uint8_t temp = ParamGet(ALS_IR_ADC_MISC);
    ParamSet(ALS_IR_ADC_MISC, temp & IR_RANGE_NORMAL);

    ParamSet(PSLED12_SELECT, LED1_OFF);

    WriteToRegister(UCOEF0, 0x7B);
    WriteToRegister(UCOEF1, 0x6B);
    WriteToRegister(UCOEF2, 0x01);
    WriteToRegister(UCOEF3, 0x00);
}

static void si1145_force()
{
    WriteToRegister(COMMAND, ALS_FORCE);
    HAL_Delay(5);
}

uint32_t si1145_getVisible()
{
    ParamSet(CHLIST, EN_ALS_VIS);
    si1145_force();
    uint16_t data = 0;
    uint8_t vis0 = ReadFromRegister(ALS_VIS_DATA0);
    uint8_t vis1 = ReadFromRegister(ALS_VIS_DATA1);
    data = vis0 | (uint16_t)(vis1 << 8);
    if(ADC_OFFSET > data)
    {
        return 0;
    }
    else
    {
        return data - ADC_OFFSET;
    }
}

uint32_t si1145_getIR()
{
    ParamSet(CHLIST, EN_ALS_IR);
    si1145_force();
    uint16_t data = 0;
    uint8_t ir0 = ReadFromRegister(ALS_IR_DATA0);
    uint8_t ir1 = ReadFromRegister(ALS_IR_DATA1);
    data = ir0 | (uint16_t)(ir1 << 8);
    if(ADC_OFFSET > data)
    {
        return 0;
    }
    else
    {
        return data - ADC_OFFSET;
    }
}

uint32_t si1145_getUVIndex()
{
    ParamSet(CHLIST, EN_UV);
    si1145_force();
    uint16_t data = 0;
    uint8_t uv0 = ReadFromRegister(UVINDEX0);
    uint8_t uv1 = ReadFromRegister(UVINDEX1);
    data = (uint16_t)uv0 | (uv1 << 8);
    return data;
}

uint8_t si1145_getResponse()
{
    return ReadFromRegister(RESPONSE);
}

void si1145_sendNOP()
{
    WriteToRegister(COMMAND, NOP);
}

char* si1145_getStatus()
{
    uint8_t status = ReadFromRegister(CHIP_STAT);
    switch(status)
    {
        case 1: return "SLEEP";
        case 2: return "SUSPEND";
        case 4: return "RUNNING";
        default: return "ERROR";
    }
}
