/**
 * @file     DS18B20.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-22
 * @brief    API for initializing ds18b20 and measuring temperature which transmitting data over a single bus
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

#if !defined(DS18B20_CONFIG)

#define DS18B20_GPIO_NUM
#define DS18B20_

#endif // DS18B20_CONFIG

#endif
