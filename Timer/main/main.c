/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-23
 * @brief    Test for Timer API to send message using UART
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/Timer.h"
#include "../BSP/include/UART1.h"
#include <stdlib.h>

char data[RX_BUF_SIZE] = {0};
uint32_t len = 0;

void app_main(void)
{

	usart_init(115200);

	while (1)
	{
		uart_write_bytes(USART_UX, "Please enter period to send message.\n", strlen("Please enter period to send message.\n"));
		vTaskDelay(1000);

		uart_get_buffered_data_len(USART_UX, (size_t *)&len);

		if (len)
		{
			uart_read_bytes(USART_UX, data, len, 10);
			int period = atoi(data);
			uart_write_bytes(USART_UX, data, strlen(data));
			if (!period)
			{
				uart_write_bytes(USART_UX, "error!\n", strlen("error!\n"));
				continue;
			}
			else
			{
				Timer_Init(period);
				break;
			}
		}
	}

	while (1)
	{
		uart_write_bytes(USART_UX, "test message\n", strlen("test message\n"));
		vTaskDelay(1000);
	}
}
