/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-18
 * @brief    Test for ADC
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/
#include "../BSP/include/ADC.h"
#include "../BSP/include/SPI_LCD.h"
#include "driver/gpio.h"
#include "../BSP/include/Convert_DemicalPlace.h"

void app_main()
{

	ADC_Init();
	SPI_LCD_Init();
	uint32_t data = 0;

	SPI_LCD_ShowString(10, 10, 11, 1, 16, "ADC result:", BLACK, WHITE);

	while (1)
	{
		data = ADC_GetResult(ADC_CHANNEL, 10); /* Head-tail trimmed average output */

		char *buf = Calculate(data);
		printf("data:%s\n", buf);
		vTaskDelay(1000);

		/* ​​Decimal part​​ */
		SPI_LCD_ShowString(10, 40, 16, 1, 16, buf, BLACK, WHITE);
	}

	return;
}
