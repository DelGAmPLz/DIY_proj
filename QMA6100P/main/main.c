/**
 * @file     hello_world_main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-24
 * @brief    test for QMA6100P to ​​Calculate the pitch and roll angles​
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "SPI_LCD.h"
#include "QMA6100P.h"
#include "stdio.h"

void ShowFloat(uint16_t x, uint16_t y, char *description, float data)
{
	int size = snprintf(NULL, 0, "%s%6.3f", description, data);
	if (size < 0)
		return;
	else if (size > 240 / 16)
	{
		ESP_LOGE("ShowFloat", " no memory!\n");
		return;
	}

	char *buf = malloc(size + 1);
	if (!buf)
	{
		ESP_LOGE("ShowFloat", " no memory!\n");
		return;
	}

	snprintf(buf, size + 1, "%s%6.3f", description, data);

	SPI_LCD_ShowString(x, y, size, 1, 16, buf, BLACK, WHITE);
	free(buf);
}

void app_main(void)
{

	SPI_LCD_Init();
	ESP_ERROR_CHECK(QMA6100P_Init());

	QMA6100P_Data_t qma_data = {0};

	while (1)
	{
		QMA6100P_ReadAll(&qma_data);
		ShowFloat(10, 10, "acc_x:", qma_data.acc_x);

		vTaskDelay(100);
	}

	return;
}