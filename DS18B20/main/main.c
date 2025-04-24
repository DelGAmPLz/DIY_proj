/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-22
 * @brief    Test for measuring environmental temperatrue using data from DS18B20
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "SPI_LCD.h"
#include "DS18B20.h"

void app_main()
{

	SPI_LCD_Init();
	DS18B20_Init();

	SPI_LCD_ShowString(10, 10, 22, 1, 16, "external temperature:", BLACK, WHITE);

	short temp = 0;

	while (1)
	{

		temp = DS18B20_GetTemp();
		SPI_LCD_ShowNum(10, 40, (uint32_t)temp, 12, 16, BLACK);
	}

	return;
}