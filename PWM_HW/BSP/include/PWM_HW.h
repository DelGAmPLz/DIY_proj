/**
 * @file     PWM_HW.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-29
 * @brief    API for LEDC to control LED with hardware fade
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _PWM_HW_H_
#define _PWM_HW_H_

#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "esp_err.h"

#define LED_GPIO_NUM GPIO_NUM_1
#define PWM_CHANNEL_NUM LEDC_CHANNEL_0
#define PWM_TIMER_NUM LEDC_TIMER_0
#define PWM_RESOLUTION LEDC_TIMER_11_BIT
#define PWM_FREQ 4000

void LEDC_HW_Init(void);

#endif