#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "../BSP/include/LED.h"

/* Definition of Ports and Pins */
#define I2C_PORT I2C_NUM_0			 /* i2c port */
#define I2C_SCL_GPIO_NUM GPIO_NUM_42 /* I2C SCL PIN */
#define I2C_SDA_GPIO_NUM GPIO_NUM_41 /* I2C SDA PIN */
#define XL9555_GPIO_NUM GPIO_NUM_0	 /* XL9555 PIN */

/* Definition of XL9555 Register commands and address */
#define XL9555_INPUT_PORT0_REG 0x00		/* read level of IO on port0 */
#define XL9555_INPUT_PORT1_REG 0x01		/* read level of IO on port1 */
#define XL9555_OUTPUT_PORT0_REG 0x02	/* write level of IO on port0 */
#define XL9555_OUTPUT_PORT1_REG 0x03	/* write level of IO on port1 */
#define XL9555_INVERSION_PORT0_REG 0x04 /* inverse level of IO on port0 */
#define XL9555_INVERSION_PORT1_REG 0x05 /* inverse level of IO on port1 */
#define XL9555_CONFIG_PORT0_REG 0x06	/* config mode of IO on port0 */
#define XL9555_CONFIG_PORT1_REG 0x07	/* config mode of IO on port1 */
#define XL9555_ADDR 0x20				/* address without read/write bit */

/* definition of Keys */
#define KEY_0 0x07
#define KEY_1 0x0b
#define KEY_2 0x0d
#define KEY_3 0x0e

const static char *TAG = "IIC_TEST\n";

i2c_master_bus_handle_t i2c_master_handler;
i2c_master_dev_handle_t i2c_masterdev_handler;

static volatile bool run_loop = false;

/**
 *
 *
 * @brief      key_scan API
 * @param      IO levels
 * @return     bool	whether certain Key is active comparing with the level
 *
 *
 **/

bool Key(uint8_t x)
{
	uint8_t r_buf[1] = {0};
	uint8_t cmd = XL9555_INPUT_PORT1_REG;
	ESP_ERROR_CHECK(i2c_master_transmit_receive(i2c_masterdev_handler, &cmd, 1, r_buf, 1, -1));
	return ((r_buf[0] >> 4) == x);
}

/**
 *
 *
 * @brief      Key_Scan task
 * @param      queue handler
 * @return     void
 *
 *
 **/

void Key_Scan(void *arg)
{
	while (run_loop)
	{
		// ESP_LOGI(TAG, "enter loop\n");

		if (Key(KEY_0))
		{
			gpio_set_level(LED_PIN, 0);
		}
		else if (Key(KEY_1))
		{
			gpio_set_level(LED_PIN, 1);
		}
		else if (Key(KEY_2))
		{
			LED_Toggle();
		}
		else if (Key(KEY_3))
		{
			gpio_set_level(LED_PIN, gpio_get_level(LED_PIN)); // 保持当前状态
		}
		else
		{
			// printf("No Key pressed!\n");
		}
		vTaskDelay(10);
	}
	ESP_LOGI(TAG, "task closed\n");
	vTaskDelete(NULL);
}

void app_main(void)
{
	/* initialize LED */

	LED_Init();

	/* initialize iic */

	i2c_master_bus_config_t i2c_master_cfg = {
		.clk_source = I2C_CLK_SRC_DEFAULT,
		.flags.enable_internal_pullup = true,
		.glitch_ignore_cnt = 7,
		.i2c_port = I2C_PORT,
		.scl_io_num = I2C_SCL_GPIO_NUM,
		.sda_io_num = I2C_SDA_GPIO_NUM,
	};
	i2c_new_master_bus(&i2c_master_cfg, &i2c_master_handler);

	i2c_device_config_t i2c_device_cfg = {
		.device_address = XL9555_ADDR,
		.scl_speed_hz = 400000,
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
	};
	i2c_master_bus_add_device(i2c_master_handler, &i2c_device_cfg, &i2c_masterdev_handler);

	/* initialize xl9555 */
	gpio_config_t xl9555_cfg = {
		.mode = GPIO_MODE_INPUT,
		.pin_bit_mask = 1ull << XL9555_GPIO_NUM,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.pull_up_en = GPIO_PULLUP_ENABLE,
		.intr_type = GPIO_INTR_DISABLE,
	};
	gpio_config(&xl9555_cfg);

	i2c_master_probe(i2c_master_handler, XL9555_ADDR, -1); /* detecte device */
	ESP_LOGI(TAG, "initialize successfully!\n");

	/* read port1 level */
	uint8_t cmd = XL9555_INPUT_PORT1_REG;
	uint8_t r_buf[1] = {0};
	i2c_master_transmit_receive(i2c_masterdev_handler, &cmd, 1, r_buf, 2, -1);
	ESP_LOGI(TAG, "port level:%hhu\n", r_buf[0]);

	/* freertos key_scan task create */
	run_loop = true;
	if (pdTRUE == xTaskCreate(Key_Scan, TAG, 4096, xTaskGetCurrentTaskHandle(), 10, NULL))
		ESP_LOGI(TAG, "task created!\n");
	else
		ESP_LOGE(TAG, "task create failed!\n");
	if (0)
		run_loop = false;
	while (1)
	{
		vTaskDelay(100);
	}
}