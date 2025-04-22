/**
 * @file     Inter_Temp_Sensor.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-21
 * @brief    Definition of API for measuring chip temperature using internal sensor
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/Inter_Temp_Sensor.h"

static bool flag = false;
static temperature_sensor_handle_t temp_sensor_handle = NULL;

esp_err_t Inter_Temp_Sensor_Init(void)
{
	temperature_sensor_config_t temp_sensor_cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(0, 30);
	return temperature_sensor_install(&temp_sensor_cfg, &temp_sensor_handle);
}

float Inter_Temp_Sensor_Get(void)
{

	float data = 0;
	if (!flag)
		temperature_sensor_enable(temp_sensor_handle);

	ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor_handle, &data));

	ESP_ERROR_CHECK(temperature_sensor_disable(temp_sensor_handle));

	return data;
}

esp_err_t Inter_Temp_Sensor_Deinit(void)
{

	if (flag)
		temperature_sensor_disable(temp_sensor_handle);
	return temperature_sensor_uninstall(temp_sensor_handle);
}
