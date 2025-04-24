/**
 * @file     RNG.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-24
 * @brief    Definition of API for RNG
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "RNG.h"

uint32_t RNG_GetNum(void)
{
	return esp_random();
}

uint32_t RNG_GetRangeNum(uint32_t min, uint32_t max)
{
	return (esp_random() % (max - min) + min);
}