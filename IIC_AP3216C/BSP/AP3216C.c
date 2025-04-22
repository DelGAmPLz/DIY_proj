/**
 * @file     AP3216C.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-20
 * @brief    Initialize AP3216C using I2C
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/AP3216C.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static i2c_master_bus_handle_t master_handler = NULL;
static i2c_master_dev_handle_t master_dev_handler = NULL;

i2c_master_bus_handle_t get_master_bus_handle(void)
{

	return master_handler;
}

i2c_master_dev_handle_t get_master_dev_handle(void)
{

	return master_dev_handler;
}

esp_err_t AP3216C_Init()
{
	// i2c_master_bus_handle_t m_handle = get_master_bus_handle();

	// i2c_master_dev_handle_t d_handle = get_master_dev_handle();

	i2c_master_bus_config_t i2c_master_cfg = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.glitch_ignore_cnt = 7,
		.i2c_port = I2C_PORT,
		.scl_io_num = I2C_SCL,
		.sda_io_num = I2C_SDA,
		.flags.enable_internal_pullup = true,
	};

	ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_cfg, &master_handler));

	i2c_device_config_t i2c_dev_cfg = {
		.device_address = AP3216C_ADDR,
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.scl_speed_hz = AP3216C_FREQ,
	};
	ESP_ERROR_CHECK(i2c_master_bus_add_device(master_handler, &i2c_dev_cfg, &master_dev_handler));

	esp_err_t err = i2c_master_probe(master_handler, AP3216C_ADDR, -1);
	if (err != ESP_OK)
		return err;

	ESP_ERROR_CHECK(AP3216C_Write_Byte(MODE_ADDR, 0x04));
	vTaskDelay(10);
	ESP_ERROR_CHECK(AP3216C_Write_Byte(MODE_ADDR, 0x03));

	return ESP_OK;
}

static esp_err_t AP3216C_Write_Byte(uint8_t reg, uint8_t data)
{
	// i2c_master_dev_handle_t d_handle = get_master_dev_handle();
	esp_err_t err;
	uint8_t *buffer = malloc(2);
	if (buffer == NULL)
		return ESP_ERR_NO_MEM;

	buffer[0] = reg;
	buffer[1] = data;

	err = i2c_master_transmit(master_dev_handler, buffer, 2, -1);

	free(buffer);
	return err;
}

static uint8_t AP3216C_Read_Byte(uint8_t reg)
{

	i2c_master_dev_handle_t d_handle = get_master_dev_handle();
	uint8_t data = 0;

	ESP_ERROR_CHECK(i2c_master_transmit_receive(d_handle, &reg, 1, &data, 1, -1));

	return data;
}

/**
 *
 *
 * @brief      read data from register of AP3216C
 *             including IR with 10-bit data,ALS with 16-bit data,PS with 10-bit data
 * @param
 * @return     uint16_t
 *
 *
 **/

esp_err_t AP3216C_Read(uint16_t *ir, uint16_t *ps, uint16_t *als)
{

	uint8_t *buf = malloc(6);
	if (buf == NULL)
		return ESP_ERR_NO_MEM;
	uint8_t i;

	for (i = 0; i < 6; i++)
	{
		buf[i] = AP3216C_Read_Byte(0X0A + i); /* 循环读取所有传感器数据 */
	}

	if (buf[0] & 0X80) /* IR_OF位为1,则数据无效 */
	{
		*ir = 0;
	}
	else
	{
		*ir = ((uint16_t)buf[1] << 2) | (buf[0] & 0X03); /* 读取IR传感器的数据   */
	}

	*als = ((uint16_t)buf[3] << 8) | buf[2]; /* 读取ALS传感器的数据   */

	if (buf[4] & 0x40) /* IR_OF位为1,则数据无效 */
	{
		*ps = 0;
	}
	else
	{
		*ps = ((uint16_t)(buf[5] & 0X3F) << 4) | (buf[4] & 0X0F); /* 读取PS传感器的数据 */
	}
	free(buf);
	vTaskDelay(120);

	return ESP_OK;
}