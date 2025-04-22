/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-29
 * @brief    Test for LEDC to control LED fade with software
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/PWM_SW.h"

static char *TAG = "ledc_example";

void app_main(void)
{

	uint64_t reso = 5, freq = 1000;

	static uint64_t max_duty = 2 * 2 * 2 * 2 * 2; // duty dynamic between 0~2^reso

	uint64_t cur_duty = 0; // record the value of duty

	LEDC_Init(reso, freq);

	ESP_LOGI(TAG, "LEDC Initialized!\n");

	while (1)
	{

		cur_duty = Get_Duty();
		ESP_LOGI(TAG, "duty is %lld\n", cur_duty);

		if (cur_duty + 5 > max_duty)
		{
			cur_duty = 0;
		}

		cur_duty += 5;
		Set_Duty(cur_duty);
		ESP_LOGI(TAG, "duty is set :%lld\n", cur_duty);

		vTaskDelay(100);
	}
}