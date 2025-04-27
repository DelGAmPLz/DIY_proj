/**
 * @file     QMA6100P.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-24
 * @brief    API for  QMA6100P to ​​Calculate the pitch and roll angles​
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _QMA6100P_H_
#define _QMA6100P_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "XL9555.h"
#include "esp_err.h"
#include "math.h"

#ifndef QMA6100P_I2C_CONFIG

#define QMA6100P_ADDR 0x12
#define QMA6100P_ID 0x90
#define QMA6100P_FREQ 400 * 1000

#define g 9.80665f
#define RED_TO_DEG (180.0f / M_PI)

typedef enum
{
	QMA6100P_MAP_INT1,
	QMA6100P_MAP_INT2,
	QMA6100P_MAP_INT_NONE
} QMA6100P_INT_MAP;

typedef enum
{
	QMA6100P_BW_100 = 0,
	QMA6100P_BW_200 = 1,
	QMA6100P_BW_400 = 2,
	QMA6100P_BW_800 = 3,
	QMA6100P_BW_1600 = 4,
	QMA6100P_BW_50 = 5,
	QMA6100P_BW_25 = 6,
	QMA6100P_BW_12_5 = 7,
	QMA6100P_BW_OTHER = 8
} QMA6100P_BW;

typedef enum
{
	QMA6100P_RANGE_2G = 0x01,
	QMA6100P_RANGE_4G = 0x02,
	QMA6100P_RANGE_8G = 0x04,
	QMA6100P_RANGE_16G = 0x08,
	QMA6100P_RANGE_32G = 0x0f
} QMA6100P_RANGE;

typedef enum
{
	QMA6100P_MCLK_102_4K = 0x03,
	QMA6100P_MCLK_51_2K = 0x04,
	QMA6100P_MCLK_25_6K = 0x05,
	QMA6100P_MCLK_12_8K = 0x06,
	QMA6100P_MCLK_6_4K = 0x07,
	QMA6100P_MCLK_RESERVED = 0xff
} QMA6100P_MCLK;

typedef enum
{
	QMA6100P_RESET = 0xB6,
	QMA6100P_RESET_END = 0x00,
} QMA6100P_S_RESET;

typedef enum
{
	QMA6100P_ACTIVE = 0x80,
	QMA6100P_ACTIVE_DIGITAL = 0x84,
	QMA6100P_STANDBY = 0x00,
} QMA6100P_POWER;

typedef struct __attribute__((packed))
{
	uint16_t raw_acc_x;
	uint16_t raw_acc_y;
	uint16_t raw_acc_z;

	float acc_x;
	float acc_y;
	float acc_z;

	float pitch;
	float roll;
} QMA6100P_Data_t;

extern i2c_master_dev_handle_t QMA6100P_handler;

#endif

#ifndef QMA6100P_REG_ADDR

#define QMA6100P_ID_ADDR 0x00
#define QMA6100P_X_TL_ADDR 0x01
#define QMA6100P_X_TH_ADDR 0x02
#define QMA6100P_Y_TL_ADDR 0x03
#define QMA6100P_Y_TH_ADDR 0x04
#define QMA6100P_Z_TL_ADDR 0x05
#define QMA6100P_Z_TH_ADDR 0x06
#define QMA6100P_S_RESET_ADDR 0x36
#define QMA6100P_RANGE_ADDR 0x0F
#define QMA6100P_BANDWIDTH_ADDR 0x10
#define QMA6100P_POWER_MNG_ADDR 0x11

#endif

esp_err_t
QMA6100P_Init(void);

esp_err_t QMA6100P_INT_Config(QMA6100P_INT_MAP int_map, bool enable);

esp_err_t
QMA6100P_ReadAll(QMA6100P_Data_t *data);

esp_err_t QMA6100P_WriteByte(uint8_t reg, uint8_t data);

esp_err_t QMA6100P_Read(uint8_t reg, uint8_t *data, size_t size);

#endif