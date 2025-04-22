/**
 * @file     test.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-22
 * @brief    Test for Using UART on ESP32S3 to control LED with PC commands
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "../BSP/include/UART1.h"
#include "../BSP/include/LED.h"
#include <stdlib.h>

/* Initialize reveive buffer */
char data_rcv[RX_BUF_SIZE] = {0};

/* Custom Commands to control LED */
const char Command[10][10] = {"LED on", "LED off"};

/* error message */
const char *err_str = "No Command matches\n\n";

void app_main(void)
{
	LED_Init();
	usart_init(115200);

	/* LED on-initial state */
	vTaskDelay(1000);
	LED_SET(0);
	vTaskDelay(1000);

	while (1)
	{
		int len = 0;
		uart_get_buffered_data_len(USART_UX, (size_t *)&len);
		/* get data length from RX FIFO */

		if (len)
		{
			/* fetch data with a defined length from RX FIFO */
			uart_read_bytes(USART_UX, data_rcv, len, 10);

			/* Write the data into ring buffer */
			uart_write_bytes(USART_UX, (const char *)data_rcv, strlen(data_rcv));
			int i = 0;

			/* compare the received data with defined commands */
			while (1)
			{

				if (!strcmp(data_rcv, Command[i]))
				{
					LED_SET(i); /* LED on/off */
					uart_write_bytes(USART_UX, (const char *)data_rcv, strlen(data_rcv));
					break;
				}

				if (!strcmp(Command[++i], "") || (i >= 10))
				{
					uart_write_bytes(USART_UX, err_str, strlen(err_str)); /* error command */
					break;
				}
			}
		}
		else
			uart_write_bytes(USART_UX, "Please Enter:", strlen("Please Enter:"));

		vTaskDelay(1000);
	}
}