/**
 * @file     TWDT.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-27
 * @brief    Definition for TWDT API
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/TWDT.h"

esp_err_t func_a(void)
{
	return esp_task_wdt_reset_user(func_a_user_handle);
}

esp_err_t func_b(void)
{
	return esp_task_wdt_reset_user(func_b_user_handle);
}

void task_func(void *arg)
{

	ESP_ERROR_CHECK(esp_task_wdt_add(NULL));

	ESP_ERROR_CHECK(esp_task_wdt_add_user("func_a", &func_a_user_handle));
	ESP_ERROR_CHECK(esp_task_wdt_add_user("func_b", &func_b_user_handle));

	UBaseType_t high_water;

	while (run_loop)
	{

		high_water = uxTaskGetStackHighWaterMark(NULL);
		ESP_LOGI(TAG, "Remaining Stack: %u", high_water);

		ESP_ERROR_CHECK(esp_task_wdt_reset());
		ESP_LOGI(TAG, "task reset!\n");

		// ESP_ERROR_CHECK(func_a());
		ESP_LOGI(TAG, "func_a reset!\n");

		ESP_ERROR_CHECK(func_b());
		ESP_LOGI(TAG, "func_b reset!\n");

		vTaskDelay(TASK_RESET_PERIOD_MS);
	}

	ESP_ERROR_CHECK(esp_task_wdt_delete(NULL));
	ESP_ERROR_CHECK(esp_task_wdt_delete_user(func_a_user_handle));
	ESP_ERROR_CHECK(esp_task_wdt_delete_user(func_b_user_handle));

	ESP_LOGI(TAG, "twdt monitor deleted all!\n");

	xTaskNotifyGive((TaskHandle_t)arg);
	vTaskDelete(NULL);
}
