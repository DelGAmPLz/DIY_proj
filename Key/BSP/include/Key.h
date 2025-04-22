/**
 * @file     Key.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    API for Key set on GPIO_0. And Key is named BOOT on the board.
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _KEY_H_
#define _KEY_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum KEY_GPIO_LEVEL
{
	KEY_RESET = 0,
	KEY_SET
};

/* definition of BOOT GPIO PIN */
#define BOOT GPIO_NUM_0

#define KEY_GET() gpio_get_level(BOOT) /* Encapsulation(abstraction)                            \
										hides the specific implementation of the underlying API \
										and provides a higher-level interface. */

void Key_Init(void);

int8_t Key_Scan(void);

#endif
