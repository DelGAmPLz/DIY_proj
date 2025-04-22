#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#define I2C_PORT I2C_NUM_0
#define I2C_SCL_GPIO_NUM GPIO_NUM_42
#define I2C_SDA_GPIO_NUM GPIO_NUM_41

#define XL9555_ADDR 0x20
#define XL9555_ADDR_WR 0x40
#define XL9555_ADDR_RD 0x41

/* XL9555命令宏 */
#define XL9555_INPUT_PORT0_REG 0x00		/* 输入寄存器0地址 */
#define XL9555_INPUT_PORT1_REG 0x01		/* 输入寄存器1地址 */
#define XL9555_OUTPUT_PORT0_REG 0x02	/* 输出寄存器0地址 */
#define XL9555_OUTPUT_PORT1_REG 0x03	/* 输出寄存器1地址 */
#define XL9555_INVERSION_PORT0_REG 0x04 /* 极性反转寄存器0地址 */
#define XL9555_INVERSION_PORT1_REG 0x05 /* 极性反转寄存器1地址 */
#define XL9555_CONFIG_PORT0_REG 0x06	/* 方向配置寄存器0地址 */
#define XL9555_CONFIG_PORT1_REG 0x07	/* 方向配置寄存器1地址 */

const static char *TAG = "IIC_TEST\n";

void app_main(void)
{

	esp_err_t ret;
	ret = nvs_flash_init(); /* 初始化NVS */
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ESP_ERROR_CHECK(nvs_flash_init());
	}

	i2c_master_bus_handle_t master_handler;

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

	i2c_master_dev_handle_t master_device_handle;

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
	gpio_set_level(GPIO_NUM_0, 1);

	int level46 = gpio_get_level(GPIO_NUM_46);
	ESP_LOGI(TAG, "46 level: %d\n", level46);
	int level0 = gpio_get_level(GPIO_NUM_0);
	ESP_LOGI(TAG, "0 level: %d\n", level0);

	ESP_LOGI(TAG, "xl9555 gpio initialized\n");

	uint8_t r_data[2] = {0};

	ESP_ERROR_CHECK(i2c_master_probe(master_handler, XL9555_ADDR, -1));

	/* config input mode */
	uint8_t cfg_buf[2] = {XL9555_CONFIG_PORT0_REG, 0x88};
	ESP_ERROR_CHECK(i2c_master_transmit(master_device_handle, cfg_buf, 2, -1));

	/* write level */
	uint8_t buf[2];
	buf[0] = XL9555_OUTPUT_PORT0_REG;
	buf[1] = 0xAA;
	ESP_ERROR_CHECK(i2c_master_transmit(master_device_handle, buf, 2, -1));
	ESP_LOGI(TAG, "port0 level:%#x\n", buf[1]);

	/* read level */
	uint8_t r_cmd = XL9555_INPUT_PORT0_REG;
	ESP_ERROR_CHECK(i2c_master_transmit_receive(master_device_handle, &r_cmd, 1, r_data, 2, -1));
	ESP_LOGI(TAG, "r_data[0]:%#x,r_data[1]:%#x\n", r_data[0], r_data[1]);

	/* get IO levels on port0 */
	/* uint8_t wr_buf[3] = {0};
	uint8_t rd_buf[3] = {0};
	wr_buf[0] = XL9555_ADDR_WR;
	wr_buf[1] = XL9555_OUTPUT_PORT0_REG;
	wr_buf[2] = XL9555_ADDR_RD;
	ESP_ERROR_CHECK(i2c_master_transmit_receive(master_device_handle, wr_buf, 3, rd_buf, 1, -1));

	ESP_LOGI(TAG, "i2c transmitted: %hhu,%hhu,%hhu\n", wr_buf[0], wr_buf[1], wr_buf[2]);
	ESP_LOGI(TAG, "i2c read:%hhu\n", rd_buf[0]); */
}