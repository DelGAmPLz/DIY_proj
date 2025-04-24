/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-24
 * @brief    test for RNG
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "RNG.h"
#include "SPI_LCD.h"

void app_main()
{
	SPI_LCD_Init();

	SPI_LCD_ShowString(10, 10, 12, 1, 24, "Random val:", BLACK, WHITE);
	SPI_LCD_ShowString(10, 100, 11, 1, 24, "10~30 val:", BLACK, WHITE);

	uint32_t count = 0;
	uint32_t rand = 0;

	while (1)
	{

		count = RNG_GetNum();
		SPI_LCD_ShowNum(160, 10, count, 6, 24, BLACK);
		rand = RNG_GetRangeNum(10, 20);
		SPI_LCD_ShowNum(150, 100, rand, 6, 24, BLACK);
	}

	return;
}