/**
 * @file     LED.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-16
 * @brief    definition for LED functions
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/LED.h"

/**
 *
 *
 * @brief      Init GPIO of LED
 * @param      no
 * @return     no
 *
 *
 **/

void LED_Init(void)
{
	gpio_config_t led_cfg_t = {0};
	led_cfg_t.mode = GPIO_MODE_INPUT_OUTPUT;
	led_cfg_t.intr_type = GPIO_INTR_DISABLE;
	led_cfg_t.pull_down_en = GPIO_PULLDOWN_DISABLE;
	led_cfg_t.pull_up_en = GPIO_PULLUP_ENABLE;
	led_cfg_t.pin_bit_mask = 1ull << LED_PIN;
	gpio_config(&led_cfg_t);
	LED_SET(GPIO_SET);
}

/**
 *
 *
 * @brief      toggle the LED
 * @param      no
 * @return     no
 *
 *
 **/
void LED_Toggle(void)
{
	LED_SET(!LED_GET);
}
