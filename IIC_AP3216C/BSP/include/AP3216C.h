/**
 * @file     AP3216C.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-20
 * @brief    API for AP3216C to read data from ALS,PS,IR
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _AP3216C_H_
#define _AP3216C_H_

#include "driver/i2c_master.h"
#include "esp_err.h"

#ifndef AP3216C_I2C_CONFIG

#define I2C_SDA GPIO_NUM_41
#define I2C_SCL GPIO_NUM_42
#define I2C_PORT I2C_NUM_1
#define AP3216C_ADDR 0x1E
#define AP3216C_FREQ 400 * 1000

#endif

#ifndef AP3216C_REG_ADDR

typedef enum
{
	MODE_ADDR = 0x00,
	IR_ADDR0 = 0x0A,
	IR_ADDR1 = 0x0B,
	ALS_ADDR0 = 0x0C,
	ALS_ADDR1 = 0x0D,
	PS_ADDR0 = 0x0E,
	PS_ADDR1 = 0x0F,
} AP3216C_Reg_Addr;

#endif

i2c_master_bus_handle_t
get_master_bus_handle(void);

i2c_master_dev_handle_t get_master_dev_handle(void);

esp_err_t AP3216C_Init();

static esp_err_t AP3216C_Write_Byte(uint8_t reg, uint8_t data);

static uint8_t AP3216C_Read_Byte(uint8_t reg);

esp_err_t AP3216C_Read(uint16_t *ir, uint16_t *ps, uint16_t *als);

#endif
