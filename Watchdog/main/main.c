/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-27
 * @brief    To avoid ​multiple definition errors in compilation, an ​unencapsulated/modified main file needs to be ​properly encapsulated before being used across multiple files.
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

/*
test for freertos
test for esp_log
*/

#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_log.h"
#include "../BSP/include/TWDT.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#define MAIN_STOPTIME_MS 10000
#define TWDT_TIMEOUT_MS 3000
#define TASK_RESET_PERIOD_MS 2000

static volatile bool run_loop;

static const char *TAG = "example";

static esp_task_wdt_user_handle_t func_a_user_handle;
static esp_task_wdt_user_handle_t func_b_user_handle;

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

void app_main(void)
{
	TAG = "example";
	esp_task_wdt_config_t wtdt_cfg = {
		.timeout_ms = TWDT_TIMEOUT_MS,
		.idle_core_mask = 1 << 0,
		.trigger_panic = false};

	ESP_ERROR_CHECK(esp_task_wdt_init(&wtdt_cfg));
	ESP_LOGI(TAG, "twdt initialized!\n");

	run_loop = true;
	xTaskCreatePinnedToCore(task_func, TAG, 4096, xTaskGetCurrentTaskHandle(), 10, NULL, 0);
	ESP_LOGI(TAG, "task created!\n");

	vTaskDelay(MAIN_STOPTIME_MS);
	run_loop = false;
	ESP_LOGI(TAG, "task closed!\n");

	if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000)) == 0)
	{
		ESP_LOGE(TAG, "子任务未在5秒内退出!");
	}

	ESP_ERROR_CHECK(esp_task_wdt_deinit());
	ESP_LOGI(TAG, "twdt deinit!\n");
}
