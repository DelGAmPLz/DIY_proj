/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    Test for EXTI of GPIO BOOT to control LED
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "driver/gpio.h"
#include "../BSP/include/Key.h"
#include "../BSP/include/LED.h"
#include "../BSP/include/EXTI.h"
#include "nvs_flash.h"

void app_main(void)
{
	/* To handle NVS partition initialization failures or firmware updates
	first erase NVS,then reinitialize it  */
	esp_err_t ret;
	ret = nvs_flash_init(); /* 初始化NVS */
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	/* ********************************************************************************* */

	EXTI_Init();
	LED_Init();

	while (1)
	{
		vTaskDelay(10);
	}
}
