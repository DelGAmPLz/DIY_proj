/**
 * @file     UART.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-17
 * @brief    API definition for URAT
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "UART1.h"

/**
 *
 *
 * @brief      Initialize for uart0
 * @param      baudrate
 * @return     no
 *
 *
 **/

void usart_init(uint32_t baudrate)
{
	uart_config_t uart_config; /* 串口配置句柄 */

	uart_config.baud_rate = baudrate;
	uart_config.data_bits = UART_DATA_8_BITS;
	uart_config.parity = UART_PARITY_DISABLE;
	uart_config.stop_bits = UART_STOP_BITS_1;
	uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
	// uart_config.source_clk = UART_SCLK_APB;			  /* default */
	// uart_config.rx_flow_ctrl_thresh = 122;			  /* default */

	/* register UART */
	uart_param_config(USART_UX, &uart_config);

	/* config UART Pin */
	uart_set_pin(USART_UX, USART_TX_GPIO_PIN, USART_RX_GPIO_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

	/* install driver of UART */
	uart_driver_install(USART_UX, RX_BUF_SIZE * 2, RX_BUF_SIZE * 2, 0, NULL, 0);
}