/**
 * @file     TWDT.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-27
 * @brief    TWDT API
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _TWDT_H_
#define _TWDT_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "esp_err.h"
#include "esp_log.h"

#define MAIN_STOPTIME_MS 10000
#define TWDT_TIMEOUT_MS 3000
#define TASK_RESET_PERIOD_MS 2000

extern bool run_loop;

extern char *TAG;

extern esp_task_wdt_user_handle_t func_a_user_handle;
extern esp_task_wdt_user_handle_t func_b_user_handle;

esp_err_t func_a(void);

esp_err_t func_b(void);

void task_func(void *arg);

#endif