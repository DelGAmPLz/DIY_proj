/**
 * @file     ADC.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-18
 * @brief    API for ADC
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _ADC_H_
#define _ADC_H_

#include "esp_adc/adc_oneshot.h"

#ifndef ADC_CHANNEL_CFG

#define ADC_CHANNEL ADC_CHANNEL_7

extern adc_oneshot_unit_handle_t adc_oneshot_handle;

#endif

#ifndef CALMAN

// 卡尔曼滤波器结构体
typedef struct
{
	float q; // 过程噪声协方差
	float r; // 测量噪声协方差
	float p; // 估计误差协方差
	float k; // 卡尔曼增益
	float x; // 最优估计值
} kalman_filter_t;

extern kalman_filter_t kalman_cfg;

#endif

esp_err_t ADC_Init(void);

uint32_t ADC_GetResult(adc_channel_t channel, int times);

void Kalman_Init(kalman_filter_t *kf, float q, float r);

float Kalman_Get(float q, float r, float measurement);

#endif