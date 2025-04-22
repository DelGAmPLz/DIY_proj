/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-21
 * @brief    test for measuring voltage using internal adc to read data from
 *           Internal Temperature Sensor
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/SPI_LCD.h"
#include "../BSP/include/XL9555.h"
#include "../BSP/include/Inter_Temp_Sensor.h"
#include <string.h>

void app_main()
{
	SPI_LCD_Init();
	Inter_Temp_Sensor_Init();

	float data = 0;

	char str[16];
	memset(str, 0, 16);

	SPI_LCD_ShowString(10, 10, 22, 1, 16, "Internal Temperature:", BLACK, WHITE);

	while (1)
	{

		data = Inter_Temp_Sensor_Get();

		printf("%f", data);
		snprintf(str, sizeof(str), "%.3f degree", data);

		SPI_LCD_ShowString(10, 40, 16, 1, 16, str, BLACK, WHITE);
		vTaskDelay(100);
	}

	return;
}