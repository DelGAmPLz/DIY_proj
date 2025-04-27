/**
 * @file     UART.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    API for UART
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _UART_H_
#define _UART_H_

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/uart_select.h"
#include "driver/gpio.h"

/*  UART Pin  */
#define USART_UX UART_NUM_0
#define USART_TX_GPIO_PIN GPIO_NUM_43
#define USART_RX_GPIO_PIN GPIO_NUM_44

/* Ring BUFFER size */
#define RX_BUF_SIZE 6000

void usart_init(uint32_t baudrate);

#endif
