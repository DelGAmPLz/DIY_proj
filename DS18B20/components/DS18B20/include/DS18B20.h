/**
 * @file     DS18B20.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-22
 * @brief    API for initializing ds18b20 and measuring temperature which transmitting data over one-wire
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "driver/gpio.h"
#include "esp_err.h"

// #if !defined(DS18B20_CONFIG)
#ifndef DS18B20_CONFIG

#define DS18B20_GPIO_NUM GPIO_NUM_0
#define DS18B20_ROM_ADDR 0xcc
#define DS18B20_CONV_CMD 0x44
#define DS18B20_READ_CMD 0xbe

#endif // DS18B20_CONFIG

#define DS18B20_Pull(x)                          \
	do                                           \
	{                                            \
		if (x)                                   \
			gpio_set_level(DS18B20_GPIO_NUM, 1); \
		gpio_set_level(DS18B20_GPIO_NUM, 0);     \
	} while (0)

#define DS18B20_Read() gpio_get_level(DS18B20_GPIO_NUM)

esp_err_t DS18B20_Init(void);

esp_err_t DS18B20_ResetCheck(void);

int DS18B20_ReadBit(void);

esp_err_t DS18B20_WriteBit(int bit);

uint8_t DS18B20_ReadByte(void);

esp_err_t DS18B20_WriteByte(uint8_t data);

short DS18B20_GetTemp(void);

#endif
