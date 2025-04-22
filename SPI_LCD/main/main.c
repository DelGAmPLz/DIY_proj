/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "../BSP/include/XL9555.h"
#include "../BSP/include/SPI_LCD.h"
#include "string.h"

void app_main(void)
{
	XL9555_Init();

	ESP_ERROR_CHECK(SPI_LCD_Init());

	SPI_LCD_Display_Dir(1);

	SPI_LCD_Background(WHITE);
	// SPI_LCD_DrawCircle(120, 120, 100, BLACK);

	SPI_LCD_ShowString(0, 10, 10, 10, 16, "test char!*********************************************************", BLUE, RED);
	while (1)
	{

		if (XL9555_Read_IO(KEY3_IO) == 0)
		{
			printf("IO read right!");
			vTaskDelay(10);
		}
	}
}
