/**
 * @file     Inter_Temp_Sensor.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-21
 * @brief    API for measuring chip temperature using internal sensor
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/
#ifndef _INTER_TEMP_SENSOR_H_
#define _INTER_TEMP_SENSOR_H_

#include "esp_err.h"
#include "driver/temperature_sensor.h"

esp_err_t Inter_Temp_Sensor_Init(void);

float Inter_Temp_Sensor_Get(void);

esp_err_t Inter_Temp_Sensor_Deinit(void);

#endif