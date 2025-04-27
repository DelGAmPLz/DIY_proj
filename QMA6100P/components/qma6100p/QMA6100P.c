/**
 * @file     QMA6100P.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-24
 * @brief    Definition of API for QMA6100P to ​​Calculate the pitch and roll angles​
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "QMA6100P.h"

i2c_master_dev_handle_t QMA6100P_handler;

esp_err_t QMA6100P_Init(void)
{
	i2c_device_config_t qma_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_7,
		.device_address = QMA6100P_ADDR,
		.scl_speed_hz = QMA6100P_FREQ,
	};

	ESP_ERROR_CHECK(i2c_master_bus_add_device(master_handler, &qma_cfg, &QMA6100P_handler));

	esp_err_t err = i2c_master_probe(master_handler, QMA6100P_ADDR, -1);
	if (err != ESP_OK)
		return err;

	uint8_t ID = 0;
	QMA6100P_Read(QMA6100P_ID_ADDR, &ID, 1);
	if (ID != QMA6100P_ID)
		return ESP_ERR_NOT_FOUND;

	ESP_ERROR_CHECK(QMA6100P_WriteByte(QMA6100P_S_RESET_ADDR, QMA6100P_RESET));
	vTaskDelay(5);
	ESP_ERROR_CHECK(QMA6100P_WriteByte(QMA6100P_S_RESET_ADDR, QMA6100P_RESET_END));
	vTaskDelay(10);

	QMA6100P_Read(QMA6100P_ID_ADDR, &ID, 1);
	if (ID != QMA6100P_ID)
		return ESP_ERR_NOT_FOUND;

	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x11, 0x80));
	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x11, 0x84));
	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x4a, 0x20));
	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x56, 0x01));
	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x5f, 0x80));
	vTaskDelay(1);
	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x5f, 0x00));
	vTaskDelay(10);

	ESP_ERROR_CHECK(QMA6100P_WriteByte(QMA6100P_RANGE_ADDR, QMA6100P_RANGE_8G));
	ESP_ERROR_CHECK(QMA6100P_WriteByte(QMA6100P_BANDWIDTH_ADDR, QMA6100P_BW_100));
	ESP_ERROR_CHECK(QMA6100P_WriteByte(QMA6100P_POWER_MNG_ADDR, QMA6100P_MCLK_51_2K | 0x80));

	ESP_ERROR_CHECK(QMA6100P_WriteByte(0x21, 0x03));

	ESP_ERROR_CHECK(QMA6100P_INT_Config(QMA6100P_MAP_INT1, 1));

	ESP_ERROR_CHECK(QMA6100P_Read(QMA6100P_ID_ADDR, &ID, 1));
	if (ID != QMA6100P_ID)
		return ESP_ERR_NOT_FOUND;

	return ESP_OK;
}

esp_err_t QMA6100P_INT_Config(QMA6100P_INT_MAP int_map, bool enable)
{
	uint8_t reg_16 = 0;
	uint8_t reg_19 = 0;
	uint8_t reg_1b = 0;

	ESP_ERROR_CHECK(QMA6100P_Read(0x16, &reg_16, 1));
	ESP_ERROR_CHECK(QMA6100P_Read(0x19, &reg_19, 1));
	ESP_ERROR_CHECK(QMA6100P_Read(0x1b, &reg_1b, 1));

	if (enable)
	{
		reg_16 |= 0x08;
		reg_19 |= 0x08;
		reg_1b |= 0x08;
		ESP_ERROR_CHECK(QMA6100P_WriteByte(0x16, reg_16));

		if (int_map == QMA6100P_MAP_INT1)
		{
			ESP_ERROR_CHECK(QMA6100P_WriteByte(0x19, reg_19));
		}
		else if (int_map == QMA6100P_MAP_INT2)
		{
			ESP_ERROR_CHECK(QMA6100P_WriteByte(0x1b, reg_1b));
		}
	}
	else
	{
		reg_16 &= (~0x08);
		reg_19 &= (~0x08);
		reg_1b &= (~0x08);

		ESP_ERROR_CHECK(QMA6100P_WriteByte(0x16, reg_16));
		ESP_ERROR_CHECK(QMA6100P_WriteByte(0x19, reg_19));
		ESP_ERROR_CHECK(QMA6100P_WriteByte(0x1b, reg_1b));
	}
	return ESP_OK;
}

esp_err_t
QMA6100P_ReadAll(QMA6100P_Data_t *data)
{
	uint8_t buffer[6] = {0};
	uint8_t xl_addr = QMA6100P_X_TL_ADDR;
	/* 	ESP_ERROR_CHECK(i2c_master_transmit_receive(QMA6100P_handler, &xl_addr, 1, &buffer[0], 2, -1));
		uint8_t xh_addr = QMA6100P_X_TH_ADDR;
		ESP_ERROR_CHECK(i2c_master_transmit_receive(QMA6100P_handler, &xh_addr, 1, &buffer[1], 2, -1));

		uint8_t yl_addr = QMA6100P_Y_TL_ADDR;
		ESP_ERROR_CHECK(i2c_master_transmit_receive(QMA6100P_handler, &yl_addr, 1, &buffer[2], 2, -1)); */
	// ESP_ERROR_CHECK(i2c_master_transmit_receive(QMA6100P_handler, &xl_addr, 1, buffer, 6, -1));
	ESP_ERROR_CHECK(QMA6100P_Read(xl_addr, buffer, 6));
	printf("%hhu,%hhu,%hhu,%hhu,%hhu,%hhu\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

	data->raw_acc_x = (buffer[1] << 6) | (buffer[0] >> 2);
	data->raw_acc_y = (buffer[3] << 6) | (buffer[2] >> 2);
	data->raw_acc_z = (buffer[5] << 6) | (buffer[4] >> 2);

	data->acc_x = (data->raw_acc_x * g) / 1024.0f;
	data->acc_y = (data->raw_acc_y * g) / 1024.0f;
	data->acc_z = (data->raw_acc_z * g) / 1024.0f;

	data->pitch = -atan2f(data->acc_x, data->acc_z) * RED_TO_DEG;

	float acc_normal = sqrtf(data->acc_x * data->acc_x + data->acc_y * data->acc_y + data->acc_z * data->acc_z);

	float acc_normal_ = sqrtf(data->acc_x / acc_normal * data->acc_x / acc_normal + data->acc_y / acc_normal * data->acc_y / acc_normal + data->acc_z / acc_normal * data->acc_z / acc_normal);

	data->roll = asinf(data->acc_y / acc_normal / acc_normal_);

	return ESP_OK;
}

esp_err_t QMA6100P_WriteByte(uint8_t reg, uint8_t data)
{
	uint8_t buffer[2] = {reg, data};
	return i2c_master_transmit(QMA6100P_handler, buffer, 2, -1);
}

esp_err_t QMA6100P_Read(uint8_t reg, uint8_t *data, size_t size)
{
	return i2c_master_transmit_receive(QMA6100P_handler, &reg, 1, data, size, -1);
}