/**
 * @file     RNG.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-24
 * @brief    API for RNG
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/
#ifndef _RNG_H_
#define _RNG_H_

#include "esp_random.h"

uint32_t RNG_GetNum(void);

uint32_t RNG_GetRangeNum(uint32_t min, uint32_t max);

#endif