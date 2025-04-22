/**
 * @file     PWM_HW.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-29
 * @brief    definition of PWM API to control LED fade with hardware
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
 * @brief      Initialize the LEDC
 * @param      void
 * @return     void
 *
 *
 **/

void LEDC_HW_Init(void)
{

	ledc_timer_config_t ledc_timer_cfg = {
		.clk_cfg = LEDC_AUTO_CLK,
		.deconfigure = false,
		.duty_resolution = PWM_RESOLUTION,
		.freq_hz = PWM_FREQ,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.timer_num = PWM_TIMER_NUM};

	ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_cfg));

	ledc_channel_config_t ledc_channel_cfg = {
		.channel = PWM_CHANNEL_NUM,
		.duty = 0,
		.flags.output_invert = 0,
		.gpio_num = LED_GPIO_NUM,
		.hpoint = 0,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.timer_sel = PWM_TIMER_NUM};

	ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_cfg));
}
