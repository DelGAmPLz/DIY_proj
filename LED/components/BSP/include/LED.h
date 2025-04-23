/**
 * @file     LED.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-16
 * @brief    API for LED to enable GPIO1 to toggle its level
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _LED_H_
#define _LED_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

enum LED_GPIO_OUTPUT_STATE
{
	GPIO_RESET = 0,
	GPIO_SET
};

/* LED PIN is set on gpio_1*/
#define LED_PIN GPIO_NUM_1

/* Simplify the interface for reading GPIO levels */
#define LED_GET gpio_get_level(LED_PIN)

/* Simplify the interface for setting GPIO levels */
#define LED_SET(x)                                                                   \
	do                                                                               \
	{                                                                                \
		x ? gpio_set_level(LED_PIN, GPIO_SET) : gpio_set_level(LED_PIN, GPIO_RESET); \
	} while (0) // do whlie loop is a better macro when macro is expanded

void LED_Init(void);
void LED_Toggle(void);

#endif
