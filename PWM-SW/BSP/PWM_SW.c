/**
 * @file     PWM_SW.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-29
 * @brief    definition of PWM API to control LED fade with software
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/PWM_SW.h"

/**
 *
 *
 * @brief      definition of initialization of LEDC(PWM)
 * @param      resolution which decide how much partion can a period devided
 *             freq which decide the length of a period
 * @return     void
 *
 *
 **/

void LEDC_Init(uint8_t resolution, uint32_t freq)
{

	ledc_timer_config_t ledc_timer_cfg = {
		.clk_cfg = LEDC_AUTO_CLK,
		.duty_resolution = resolution,
		.freq_hz = freq,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.timer_num = LEDC_TIMER_NUM,
	};

	ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer_cfg));

	ledc_channel_config_t ledc_channel_cfg = {
		.channel = LEDC_CHANNEL,
		.duty = 0,
		.flags.output_invert = 0,
		.gpio_num = LED_GPIO_NUM,
		.hpoint = 0,
		.intr_type = LEDC_INTR_DISABLE,
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.timer_sel = LEDC_TIMER_NUM};

	ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel_cfg));
}

/**
 *
 *
 * @brief      set duty of pwm
 * @param      uint32_t duty
 * @return     void
 *
 *
 **/

void Set_Duty(uint32_t duty)
{
	ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL, duty));
	ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL));
}

/**
 *
 *
 * @brief      get current duty of pwm
 * @param      void
 * @return     duty
 *
 *
 **/

uint32_t Get_Duty()
{
	return ledc_get_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL);
}
