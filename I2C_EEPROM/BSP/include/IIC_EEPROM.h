/**
 * @file     IIC_EEPROM.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-05
 * @brief    API for I2C comunicate with EEPROM
 * 			 EEPROM:24C02 page wrtie size:1kb,page cache size:8kb
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "driver/i2c_master.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "string.h"

/* i2c param definition */
#define I2C_PORT I2C_NUM_1
#define I2C_SDA_GPIO_NUM GPIO_NUM_41
#define I2C_SCL_GPIO_NUM GPIO_NUM_42

/* eeprom param definition */
#define EEPROM_ADDR_MAX 255
#define EEPROM_ADDR 0x50
#define EEPROM_SPEED_HZ 400000

esp_err_t I2C_EEPROM_Init(void);

esp_err_t EEPROM_Write_Page(uint8_t *w_buf, size_t size, uint8_t w_addr);

esp_err_t EEPROM_Write_Byte(uint8_t data, uint8_t w_addr);

esp_err_t EEPROM_Read_Byte(uint8_t *r_buf, uint8_t addr);

esp_err_t EEPROM_Read_Page(uint8_t *r_buf, size_t size, uint8_t addr);
