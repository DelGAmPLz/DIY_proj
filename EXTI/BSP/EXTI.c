/**
 * @file     EXTI.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    EXTI definition for BOOT to control LED
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/EXTI.h"

/**
 *
 *
 * @brief      Initialize GPIO for BOOT
 * @param      no
 * @return     no
 *
 *
 **/

void EXTI_Init(void)
{
	gpio_config_t BOOT_EXTI_cfg;
	BOOT_EXTI_cfg.mode = GPIO_MODE_INPUT;
	BOOT_EXTI_cfg.pin_bit_mask = 1ull << BOOT;
	BOOT_EXTI_cfg.intr_type = GPIO_INTR_ANYEDGE;
	BOOT_EXTI_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
	BOOT_EXTI_cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_config(&BOOT_EXTI_cfg);

	/*  */
	gpio_install_isr_service(ESP_INTR_FLAG_EDGE);

	/*  */
	gpio_isr_handler_add(BOOT, gpio_isr_handler, (void *)BOOT);

	/*  */
	gpio_intr_enable(BOOT);
}

/**
 *
 *
 * @brief      handler function of EXTI
 * 			   avoid being used in another files
 * 			   keep handler function in IRAM
 * 			   @attention:together with EXTI_Init()
 * @param      void *arg		// you can use it to pass struct pointer or sth else
 * @return     no
 *
 *
 **/

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
	uint32_t gpio_num = (uint32_t)arg;
	if (gpio_num == BOOT)
		LED_Toggle();
}
