/**
 * @file     Timer.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-23
 * @brief    header file for Timer API
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _TIMER_H_
#define _TIMER_H_

#include "esp_timer.h"
#include "../BSP/include/UART1.h"

void Timer_Init(uint64_t period);

void Call_Back(void *arg);

#endif
