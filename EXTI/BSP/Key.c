/**
 * @file     KEY.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    API for KEY set on gpio_0
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/Key.h"

/**
 *
 *
 * @brief      Initialize GPIO configuration of Key
 * @param      no
 * @return     no
 *
 *
 **/
void Key_Init(void)
{
	gpio_config_t Key_cfg;
	Key_cfg.pin_bit_mask = 1ull << BOOT;
	Key_cfg.mode = GPIO_MODE_INPUT;
	Key_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
	Key_cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
	Key_cfg.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&Key_cfg);
}

/**
 *
 *
 * @brief      Scan the level of Key(BOOT button)
 * @param      no
 * @return     GPIO level
 *
 *
 **/

int8_t Key_Scan(void)
{

	int8_t level = KEY_SET; /*  Starting from a high level */

	if (!KEY_GET())
	{
		vTaskDelay(10);
		if (!KEY_GET())
			level = KEY_RESET; /* Software debouncing */
	}
	return level;
}
