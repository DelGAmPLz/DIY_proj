/**
 * @file     EXTI.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    API for EXTI to control the LED with BOOT
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _EXTI_H_
#define _EXTI_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../main/Key.h" /* avoid repeat definition of PIN of BOOT */

/* main.c call back function */
// typedef void (*call_back)(void *arg);

void EXTI_Init(void *arg);

void IRAM_ATTR gpio_isr_handler(void *arg);

#endif
