/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-16
 * @brief    Test for toggling the LED
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "LED.h"

void app_main(void)
{
	/**
	 *
	 * @brief      Segment below is to
	 * 			   Ensure the proper initialization of non-volatile storage-NVS
	 *
	 **/

	/* 	esp_err_t ret;

		ret = nvs_flash_init();

		if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
		{
			ESP_ERROR_CHECK(nvs_flash_erase());
			ret = nvs_flash_init();
		} */

	/* **************************************** Function Part *************************************** */

	LED_Init();

	while (1)
	{
		LED_Toggle();
		printf("LED_toggled!\n");
		vTaskDelay(1000);
	}
}
