/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-20
 * @brief    test for AP3216C
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/AP3216C.h"
#include "../BSP/include/SPI_LCD.h"
#include "../BSP/include/XL9555.h"
#include <stdio.h>

void app_main()
{
	ESP_ERROR_CHECK(SPI_LCD_Init());
	ESP_ERROR_CHECK(AP3216C_Init());

	SPI_LCD_ShowString(10, 10, 19, 1, 16, "AP3216C test data:", BLACK, WHITE);
	SPI_LCD_ShowString(10, 40, 3, 1, 16, "IR:", BLACK, WHITE);
	SPI_LCD_ShowString(10, 70, 3, 1, 16, "PS:", BLACK, WHITE);
	SPI_LCD_ShowString(10, 100, 4, 1, 16, "ALS:", BLACK, WHITE);

	uint16_t ir = 0, ps = 0, als = 0;

	esp_err_t err;

	while (1)
	{
		ESP_ERROR_CHECK(AP3216C_Read(&ir, &ps, &als));

		SPI_LCD_ShowNum(40, 40, (uint32_t)ir, 10, 16, BLACK);
		SPI_LCD_ShowNum(40, 70, (uint32_t)ps, 10, 16, BLACK);
		SPI_LCD_ShowNum(40, 100, (uint32_t)als, 10, 16, BLACK);
	}
	return;
}