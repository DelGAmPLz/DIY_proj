#ifndef _PWM_SW_H_
#define _PWM_SW_H_

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO_NUM GPIO_NUM_1
#define LEDC_TIMER_NUM LEDC_TIMER_0
#define LEDC_CHANNEL LEDC_CHANNEL_0

void LEDC_Init(uint8_t resolution, uint32_t freq);

void Set_Duty(uint32_t duty);

uint32_t Get_Duty();

#endif