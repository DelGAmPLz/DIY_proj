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

#include "../components/spilcd/include/SPI_LCD.h"

void app_main()
{

	SPI_LCD_Init();

	return;
}