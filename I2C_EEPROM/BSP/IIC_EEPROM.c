#include "../BSP/include/IIC_EEPROM.h"

/* master handler */
i2c_master_bus_handle_t master_handler;

/* device handler */
i2c_master_dev_handle_t m_device_handler;

/**
 *
 *
 * @brief      initialize I2C master and add EEPROM device
 * @param      void
 * @return     esp_err_t
 *
 *
 **/

esp_err_t I2C_EEPROM_Init(void)
{
	/* initialize i2c (i2c_num_2)*/
	i2c_master_bus_config_t master_cfg = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.flags.enable_internal_pullup = true,
		.glitch_ignore_cnt = 7,
		.i2c_port = I2C_PORT,
		.scl_io_num = I2C_SCL_GPIO_NUM,
		.sda_io_num = I2C_SDA_GPIO_NUM,
	};
	i2c_new_master_bus(&master_cfg, &master_handler);

	/* initialize eeprom device */
	i2c_device_config_t device_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_7,
		.device_address = EEPROM_ADDR,
		.scl_speed_hz = EEPROM_SPEED_HZ,
	};
	i2c_master_bus_add_device(master_handler, &device_cfg, &m_device_handler);
	return i2c_master_probe(master_handler, EEPROM_ADDR, -1);
}

/**
 *
 *
 * @brief      write data on EEPROM page which write only one byte once but can cache for 8 bytes
 * @param      uint8_t *data, data to write in
 * 			   size_t size	size of data
 * @return     esp_err_t
 *
 *
 **/

esp_err_t EEPROM_Write_Page(uint8_t *w_buf, size_t size, uint8_t w_addr)
{
	if (size <= 0)
		return ESP_ERR_INVALID_ARG;

	while (size)
	{
		if (size >= 8)
		{
			uint8_t *data = malloc(size + 1);
			data[0] = w_addr;
			memcpy(data + 1, w_buf, size);
			ESP_ERROR_CHECK(i2c_master_transmit(m_device_handler, data, 8, -1));
			w_buf += 8;
			size -= 8;
			w_addr += 8;
			vTaskDelay(1);
			free(data);
			continue;
		}
		else if (size > 0 && size < 8)
		{
			uint8_t *data = malloc(size + 1);
			data[0] = w_addr;
			memcpy(data + 1, w_buf, size);
			ESP_ERROR_CHECK(i2c_master_transmit(m_device_handler, data, size, -1));
			vTaskDelay(1);
			free(data);
			break;
		}
	}
	return ESP_OK;
}

esp_err_t EEPROM_Write_Byte(uint8_t data, uint8_t w_addr)
{
	return i2c_master_transmit(m_device_handler, &data, 1, -1);
}

esp_err_t EEPROM_Read_Byte(uint8_t *r_buf, uint8_t addr)
{
	return i2c_master_transmit_receive(m_device_handler, &addr, 1, r_buf, 1, -1);
}

/**
 *
 *
 * @brief      read data from EEPROM page which read only one byte once but can cache for 8 bytes
 * @param      uint8_t *r_buf,	data to store
 * 			   size_t size,
 * 			   uint8_t addr
 * @return     return value
 *
 *
 **/

esp_err_t EEPROM_Read_Page(uint8_t *r_buf, size_t size, uint8_t addr)
{
	if (size <= 0)
		return ESP_ERR_INVALID_ARG;
	while (size)
	{
		if (size >= 8)
		{
			ESP_ERROR_CHECK(i2c_master_transmit_receive(m_device_handler, &addr, 1, r_buf, 8, -1));
			int i = 0;
			while (i < 8)
			{
				printf("size>=8:%c	", r_buf[i++]);
			}
			vTaskDelay(1);
			r_buf += 8;
			size -= 8;
			addr += 8;
			continue;
		}
		else if (size > 0 && size < 8)
		{
			ESP_ERROR_CHECK(i2c_master_transmit_receive(m_device_handler, &addr, 1, r_buf, size, -1));
			vTaskDelay(1);
			int j = 0;
			while (j < size)
			{
				printf("size<8:%c	", r_buf[j++]);
			}
			break;
		}
	}
	return ESP_OK;
}