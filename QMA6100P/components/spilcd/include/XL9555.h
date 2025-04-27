#ifndef _XL9555_H_
#define _XL9555_H_

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* I2C PINS */
#define I2C_PORT I2C_NUM_0
#define I2C_SCL_GPIO_NUM GPIO_NUM_42
#define I2C_SDA_GPIO_NUM GPIO_NUM_41

/* XL9555 ADDR */
#define XL9555_ADDR 0x20
#define XL9555_ADDR_WR 0x40
#define XL9555_ADDR_RD 0x41

/* XL9555 IO NUM */
typedef enum
{
	AP_INT_IO = 0x0001,
	QMA_INT_IO = 0x0002,
	SPK_EN_IO = 0x0004,
	BEEP_IO = 0x0008,
	OV_PWDN_IO = 0x0010,
	OV_RESET_IO = 0x0020,
	GBC_LED_IO = 0x0040,
	GBC_KEY_IO = 0x0080,
	LCD_BL_IO = 0x0100,
	CT_RST_IO = 0x0200,
	SLCD_RST_IO = 0x0400,
	SLCD_PWR_IO = 0x0800,
	KEY3_IO = 0x1000,
	KEY2_IO = 0x2000,
	KEY1_IO = 0x4000,
	KEY0_IO = 0x8000,
} IO_NUM;

/* #define AP_INT_IO 0x0001
#define QMA_INT_IO 0x0002
#define SPK_EN_IO 0x0004
#define BEEP_IO 0x0008
#define OV_PWDN_IO 0x0010
#define OV_RESET_IO 0x0020
#define GBC_LED_IO 0x0040
#define GBC_KEY_IO 0x0080
#define LCD_BL_IO 0x0100
#define CT_RST_IO 0x0200
#define SLCD_RST_IO 0x0400
#define SLCD_PWR_IO 0x0800
#define KEY3_IO 0x1000
#define KEY2_IO 0x2000
#define KEY1_IO 0x4000
#define KEY0_IO 0x8000 */

#define IO_OUTPUT_MIN 0x0004
#define IO_OUTPUT_MAX 0x0800

/* XL9555 COMMAND */
#define XL9555_INPUT_PORT0_REG 0x00		/* 输入寄存器0地址 */
#define XL9555_INPUT_PORT1_REG 0x01		/* 输入寄存器1地址 */
#define XL9555_OUTPUT_PORT0_REG 0x02	/* 输出寄存器0地址 */
#define XL9555_OUTPUT_PORT1_REG 0x03	/* 输出寄存器1地址 */
#define XL9555_INVERSION_PORT0_REG 0x04 /* 极性反转寄存器0地址 */
#define XL9555_INVERSION_PORT1_REG 0x05 /* 极性反转寄存器1地址 */
#define XL9555_CONFIG_PORT0_REG 0x06	/* 方向配置寄存器0地址 */
#define XL9555_CONFIG_PORT1_REG 0x07	/* 方向配置寄存器1地址 */

/* XL9555 master bus handler */
extern i2c_master_bus_handle_t master_handler;

int Find_IO(IO_NUM IO);

esp_err_t XL9555_Read_All(uint8_t *r_buf);

int XL9555_Read_IO(IO_NUM IO);

esp_err_t XL9555_Write(uint8_t addr, uint8_t *data, size_t len);

esp_err_t XL9555_Write_IO(IO_NUM IO, int val);

esp_err_t XL9555_IO_Mode(uint16_t mode);

esp_err_t XL9555_Init(void);

void test();
void test_();

#endif