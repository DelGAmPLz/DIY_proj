/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    Test for LED-API
 * 			 When BOOT is pressed ,enter loop
 * 			 loop break with BOOT loosed
 * 			 and LED toggle when loosed
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "driver/gpio.h"
#include "../BSP/include/Key.h"
#include "../BSP/include/LED.h"
#include "nvs_flash.h"

void app_main(void)
{
	esp_err_t ret;
	ret = nvs_flash_init(); /* 初始化NVS */
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	/* ********************************************************************************* */

	Key_Init();
	LED_Init();

	while (1)
	{

		while (!Key_Scan())
		{
			printf("Key is pressed %d!\n", Key_Scan());
			if (Key_Scan())
				break;
		}
		printf("Key is loosed %d!\n", Key_Scan());
		LED_Toggle();
	}
}
