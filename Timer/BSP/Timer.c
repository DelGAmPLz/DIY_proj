/**
 * @file     Timer.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-23
 * @brief    API definition for Timer
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/Timer.h"

/**
 *
 *
 * @brief      Definition fo Callback function to test UART sending message controled by Timer
 * @param      void *arg from esp_timer_create_args_t
 * @return     void
 *
 *
 **/

void Call_Back(void *arg)
{

	uart_write_bytes(USART_UX, "Timer Message\n", strlen("Timer Message\n"));
}

/**
 *
 *
 * @brief      Initialize Timer and call-back function
 * @param      custom period
 * @return     void
 *
 *
 **/

void Timer_Init(uint64_t period)
{

	esp_timer_handle_t timer_handler;

	esp_timer_create_args_t timer_create_st = {
		.callback = &Call_Back,
		.arg = NULL,
	};

	esp_timer_create(&timer_create_st, &timer_handler);

	esp_timer_start_periodic(timer_handler, period);
}
