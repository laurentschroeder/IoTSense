/*
 * ambient_light_sensor.h
 *
 *  Created on: 18.12.2017
 *      Author: Laurent
 */

#ifndef AMBIENT_LIGHT_SENSOR_H_
#define AMBIENT_LIGHT_SENSOR_H_

#define SLAVE_ADDRESS 0x60

// registers
#define COMMAND_REG     0x18
#define PARAM_RD_REG    0x2E
#define PARAM_WR_REG    0x17
#define HW_KEY_REG      0x07
#define RESPONSE_REG    0x20
#define ALS_VIS_DATA0   0x22
#define ALS_VIS_DATA1   0x23

// commands
#define PARAM_QUERY     0x80
#define PARAM_SET       0xA0
#define RESET           0x01
#define ALS_FORCE       0x06

// RAM address and parameters
#define CHLIST          0x01    //adress of CHLIST
#define EN_UV           0x80
#define EN_AUX          0x40
#define EN_ALS_IR       0x20
#define EN_ALS_VIS      0x10
#define EN_PS1          0x01

#define ALS_VIS_ADC_COUNTER 0x10    //address of ALS_VIS_ADC_COUNTER
//The recommended VIS_ADC_REC value is the one’s complement of ALS_VIS_ADC_GAIN.
#define ADC_Clock_1    0x00    //50 ns times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_7    0x10    //350 ns times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_15   0x20    //750 ns times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_31   0x30    //1.55 μs times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_63   0x40    //3.15 μs times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_127  0x50    //6.35 μs times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_255  0x60    //12.75 μs times 2^ALS_VIS_ADC_GAIN
#define ADC_Clock_511  0x70    //25.55 μs times 2^ALS_VIS_ADC_GAIN

#define ALS_VIS_ADC_GAIN    0x11    //adress of ALS_VIS_ADC_GAIN
#define ADC_CLOCK_DIV1      0x00
#define ADC_CLOCK_DIV16     0x04
#define ADC_CLOCK_DIV64     0x06
#define ADC_CLOCK_DIV128    0x07

#define ALS_VIS_ADC_MISC    0x12
#define VIS_RANGE_NORMAL    0x00
#define VIS_RANGE_HIGH      0x20

/* private functions */

static uint8_t ReadFromRegister(uint8_t reg);

static void WriteToRegister(uint8_t reg, uint8_t value);

static void ParamSet(uint8_t address, uint8_t value);

static uint8_t ParamGet(uint8_t address);



/* public functions */

void reset();

void init();

uint16_t GetVisible();

void SendNOP();

#endif /* AMBIENT_LIGHT_SENSOR_H_ */
