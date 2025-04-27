#include "XL9555.h"
#include "string.h"

/* master and device handler */
i2c_master_bus_handle_t master_handler;
i2c_master_dev_handle_t master_device_handle;

int Find_IO(IO_NUM IO)
{
	int i = 0;
	while (IO != 1)
	{
		IO = IO >> 1;
		i++;
	}
	return i;
}

esp_err_t XL9555_Read_All(uint8_t *r_buf)
{
	uint8_t cmd1 = XL9555_INPUT_PORT1_REG;
	uint8_t cmd0 = XL9555_INPUT_PORT0_REG;

	ESP_ERROR_CHECK(i2c_master_transmit_receive(master_device_handle, &cmd1, 1, r_buf, 1, -1));
	ESP_ERROR_CHECK(i2c_master_transmit_receive(master_device_handle, &cmd0, 1, r_buf + 1, 1, -1));

	return ESP_OK;
}
int XL9555_Read_IO(IO_NUM IO)
{
	uint8_t r_buf[2] = {0};
	XL9555_Read_All(r_buf);
	int i = Find_IO(IO);
	// printf("i:%d", i);

	uint16_t data = (r_buf[0] << 8) | r_buf[1];
	return ((data >> i) & 1);
}
esp_err_t XL9555_Write(uint8_t addr, uint8_t *data, size_t len)
{
	uint8_t *w_buf = malloc(len + 1);
	w_buf[0] = addr;
	memcpy(w_buf + 1, data, len);
	// printf("data:%,%hhu\n", *((uint8_t *)data), *((uint8_t *)data + 1));
	return i2c_master_transmit(master_device_handle, w_buf, len + 1, -1);
}
esp_err_t XL9555_Write_IO(IO_NUM IO, int val)
{
	uint8_t r_buf[2] = {0};
	uint8_t w_buf[2] = {0};
	XL9555_Read_All(r_buf);
	uint16_t data = (r_buf[0] << 8) | r_buf[1];
	int i = Find_IO(IO);
	esp_err_t err;
	if (val)
	{
		data = data | (1 << i);
		w_buf[1] = (uint8_t)(0x00 | (data >> 8));
		w_buf[0] = (uint8_t)(0x00 | data);
		printf("i:%d,data:%#x\n", i, data);
		err = XL9555_Write(XL9555_OUTPUT_PORT0_REG, w_buf, 2);
	}
	else
	{
		data = data & (~(1 << i));
		w_buf[1] = (uint8_t)(0x00 | (data >> 8));
		w_buf[0] = (uint8_t)(0x00 | data);
		printf("i:%d,data:%#x\n", i, data);
		err = XL9555_Write(XL9555_OUTPUT_PORT0_REG, w_buf, 2);
	}

	return err;
}

esp_err_t XL9555_IO_Mode(uint16_t mode)
{
	uint8_t data[2] = {0};
	data[1] = (uint8_t)(0x00 | (mode >> 8));
	data[0] = (uint8_t)(0x00 | mode);
	return XL9555_Write(XL9555_CONFIG_PORT0_REG, data, 2);
}

esp_err_t XL9555_Init(void)
{

	/* config iic */
	i2c_master_bus_config_t i2c_master_cfg = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.glitch_ignore_cnt = 7,
		.i2c_port = I2C_PORT,
		.scl_io_num = I2C_SCL_GPIO_NUM,
		.sda_io_num = I2C_SDA_GPIO_NUM,
		.flags.enable_internal_pullup = true,
	};
	ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_cfg, &master_handler));
	i2c_device_config_t i2c_device_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_7,
		.device_address = XL9555_ADDR,
		.scl_speed_hz = 400000,
	};

	ESP_ERROR_CHECK(i2c_master_bus_add_device(master_handler, &i2c_device_cfg, &master_device_handle));

	/* initialize xl9555 */
	gpio_config_t xl9555_gpio_cfg = {
		.mode = GPIO_MODE_INPUT,
		.pin_bit_mask = 1ull << GPIO_NUM_0,
		.pull_up_en = GPIO_PULLUP_ENABLE,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&xl9555_gpio_cfg);

	esp_err_t err = i2c_master_probe(master_handler, XL9555_ADDR, -1);

	if (ESP_OK != err)
		return err;
	else
	{
		return XL9555_IO_Mode(0xF00F);
	}
}

/* uint16_t XL9555_Read_Mode()
{
	uint16_t buf = 0;
	uint8_t cmd = XL9555_CONFIG_PORT0_REG;
	esp_err_t err = ESP_OK;
	do
	{
		err = i2c_master_transmit_receive(master_device_handle, &cmd, 1, buf, 2, -1);
	} while (err != ESP_OK);
	return buf;
}

bool IO_MODE_SEL(uint16_t IO){

	if (IO>=IO_OUTPUT_MIN || IO <= IO_OUTPUT_MAX)
		return 1;
	else
		return 0;
} */

void test()
{
	/* read io mode */
	uint8_t cmd1 = XL9555_CONFIG_PORT0_REG;
	uint8_t cmd2 = XL9555_CONFIG_PORT1_REG;
	uint8_t r_buf[2] = {0};

	i2c_master_transmit_receive(master_device_handle, &cmd1, 1, r_buf, 1, -1);
	i2c_master_transmit_receive(master_device_handle, &cmd2, 1, r_buf + 1, 1, -1);
	printf("port0:%hhu\n", r_buf[0]);
	printf("port1:%hhu\n", r_buf[1]);
}

void test_()
{
	uint8_t cmd1 = XL9555_CONFIG_PORT0_REG;
	uint8_t cmd2 = XL9555_CONFIG_PORT1_REG;

	uint8_t w_buf[4] = {cmd1, 0xF0, cmd2, 0xFF};

	i2c_master_transmit(master_device_handle, w_buf, 2, -1);
	i2c_master_transmit(master_device_handle, w_buf + 2, 2, -1);
}