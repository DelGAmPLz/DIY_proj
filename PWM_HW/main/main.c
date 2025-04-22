/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-29
 * @brief    test for PWM-HW API to control LED fade down and up in a period
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/PWM_HW.h"

/**
 *
 *
 * @brief      ledc callback function which is mean to get message of a completion of event
 *             and should be set in IRAM with the functions called
 * @param      param to keep flag of if the operation is finished ,user_arg to pass context message
 * @return     bool judging if there is a higher privilige task which will be awoken after callback function
 *
 *
 **/

static bool IRAM_ATTR LEDC_Cb(const ledc_cb_param_t *param, void *user_arg)
{
	BaseType_t awokentask = pdFALSE;

	if (param->event == LEDC_FADE_END_EVT)
	{
		SemaphoreHandle_t semaphore = (SemaphoreHandle_t)user_arg;
		xSemaphoreGiveFromISR(user_arg, &awokentask);
	}

	return (awokentask == pdTRUE);
}

static char *TAG = "LEDC_HW_TEST:";

void app_main(void)
{

	LEDC_HW_Init();

	/* enable fade before set pwm duty */
	ESP_ERROR_CHECK(ledc_fade_func_install(0));

	/* after enabling fade ,config and register a callback function to get message of the completion of a fade */
	SemaphoreHandle_t semaphore = xSemaphoreCreateCounting(1, 0); // create a semaphore to wait fade
	ledc_cbs_t ledc_cb_t = {
		.fade_cb = LEDC_Cb};
	ESP_ERROR_CHECK(ledc_cb_register(PWM_CHANNEL_NUM, LEDC_LOW_SPEED_MODE, &ledc_cb_t, (void *)semaphore));

	while (1)
	{
		/* LED is effect with high level ,set full duty to make it dark,empty duty to make it light */
		ESP_LOGI(TAG, "LED fade up!\n");
		ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_NUM, 2048, 5000);
		ledc_fade_start(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_NUM, LEDC_FADE_NO_WAIT);

		// semaphore arrives at task from ISR,use xsemaphoretake but not xsemaphoretakefromisr
		xSemaphoreTake(semaphore, portMAX_DELAY);
		ESP_LOGI(TAG, "LED fade up over!\n");

		ESP_LOGI(TAG, "LED fade down!\n");
		ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_NUM, 0, 5000);
		ledc_fade_start(LEDC_LOW_SPEED_MODE, PWM_CHANNEL_NUM, LEDC_FADE_NO_WAIT);

		xSemaphoreTake(semaphore, portMAX_DELAY);
		ESP_LOGI(TAG, "LED fade down over!\n");

		vTaskDelay(1000);
	}
}
