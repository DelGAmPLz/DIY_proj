/**
 * @file     Convert_DemicalPlace.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-21
 * @brief    Definition of API for converting experimental data of type uint32
 *           into a string with three decimal places.
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "Convert_DemicalPlace.h"

/* string */
char buffer[16];

/**
 *
 *
 * @brief      Convert experimental data of type uint32
 *             into a string with three decimal places.
 * @param      uint32_t data
 * @return     char* result of conversion
 *
 *
 **/

char *Calculate(uint32_t data)
{

	memset(buffer, 0, 16);

	const float V_ref = 3.3f; // ADC参考电压
	const int N = 12;		  // ADC位数

	// 1. 计算实际电压值（浮点）
	float voltage = (data / 4095.0f) * V_ref;

	// 2. 缩放为整数（四舍五入保留三位小数）
	uint32_t scaled_value = (uint32_t)(voltage * 1000.0f + 0.5f);

	// 3. 格式化字符串
	if (scaled_value < 1000)
	{
		snprintf(buffer, sizeof(buffer), "0.%03lu", scaled_value);
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "%lu", scaled_value);
		int len = strlen(buffer);
		memmove(buffer + len - 2, buffer + len - 3, 4); // 移动后三位
		buffer[len - 3] = '.';
	}
	return buffer;
}