/**
 * @file     DS18B20.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-23
 * @brief    Definition of API for initializing ds18b20
 *           and measuring temperature which transmitting data over 1-wire
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "DS18B20.h"
#include "esp_rom_sys.h"

esp_err_t DS18B20_ResetCheck(void)
{
	DS18B20_Pull(0);
	esp_rom_delay_us(500);
	DS18B20_Pull(1);
	esp_rom_delay_us(60);

	if (!DS18B20_Read())
	{
		int count = 0;
		while (!DS18B20_Read() && count <= 240)
		{
			count++;
			esp_rom_delay_us(1);
		}
		return ESP_OK;
		if (count > 240)
			return ESP_ERR_NOT_FOUND;
		else
			return ESP_OK;
	}

	else
		return ESP_ERR_NOT_FOUND;
}

esp_err_t DS18B20_Init(void)
{
	gpio_config_t DS18B20_gpio_cfg = {
		.intr_type = GPIO_INTR_DISABLE,
		.mode = GPIO_MODE_INPUT_OUTPUT_OD,
		.pin_bit_mask = 1ull << DS18B20_GPIO_NUM,
		.pull_down_en = GPIO_PULLDOWN_DISABLE,
		.pull_up_en = GPIO_PULLUP_ENABLE,
	};
	gpio_config(&DS18B20_gpio_cfg);
	return DS18B20_ResetCheck();
}

int DS18B20_ReadBit(void)
{

	DS18B20_Pull(0);
	esp_rom_delay_us(2);
	DS18B20_Pull(1);
	esp_rom_delay_us(12);
	int bit = DS18B20_Read();
	printf("bit:%d\n", bit);
	esp_rom_delay_us(50);
	return bit;
}

esp_err_t DS18B20_WriteBit(int bit)
{
	if (bit)
	{
		DS18B20_Pull(0);
		esp_rom_delay_us(2);
		DS18B20_Pull(1);
		esp_rom_delay_us(60);
		return ESP_OK;
	}
	else if (!bit)
	{
		DS18B20_Pull(0);
		esp_rom_delay_us(60);
		DS18B20_Pull(1);
		esp_rom_delay_us(2);
		return ESP_OK;
	}
	else
		return ESP_ERR_INVALID_ARG;
}

uint8_t DS18B20_ReadByte(void)
{
	uint8_t data = 0;
	for (int i = 0; i < 8; i++)
	{
		data |= (DS18B20_ReadBit() << i);
	}
	return data;
}

esp_err_t DS18B20_WriteByte(uint8_t data)
{

	for (int i = 1; i <= 8; i++)
	{
		int bit = (data & 0x01);

		if (DS18B20_WriteBit(bit) != ESP_OK)
			return ESP_ERR_NOT_FINISHED;
		data = data >> 1;
	}
	return ESP_OK;
}

short DS18B20_GetTemp(void)
{
	uint8_t flag = 1; /* 默认温度为正数 */
	uint8_t TL, TH;
	short temp;

	ESP_ERROR_CHECK(DS18B20_ResetCheck());
	DS18B20_WriteByte(DS18B20_ROM_ADDR);
	DS18B20_WriteByte(DS18B20_CONV_CMD); /*  ds1820 start convert */
	ESP_ERROR_CHECK(DS18B20_ResetCheck());

	DS18B20_WriteByte(DS18B20_ROM_ADDR); /*  skip rom */
	DS18B20_WriteByte(DS18B20_READ_CMD); /*  convert */
	TL = DS18B20_ReadByte();			 /*  LSB */
	TH = DS18B20_ReadByte();			 /*  MSB */

	printf("TL:%hhu,TH:%hhu", TL, TH);

	if (TH > 7)
	{ /* 温度为负，查看DS18B20的温度表示法与计算机存储正负数据的原理一致：
		 正数补码为寄存器存储的数据自身，负数补码为寄存器存储值按位取反后+1
		 所以我们直接取它实际的负数部分，但负数的补码为取反后加一，但考虑到低位可能+1后有进位和代码冗余，
		 我们这里先暂时没有作+1的处理，这里需要留意 */
		TH = ~TH;
		TL = ~TL;
		flag = 0;
	}

	temp = TH; /* 获得高八位 */
	temp <<= 8;
	temp += TL; /* 获得底八位 */

	/* 转换成实际温度 */
	if (flag == 0)
	{ /* 将温度转换成负温度，这里的+1参考前面的说明 */
		temp = (double)(temp + 1) * 0.625;
		temp = -temp;
	}
	else
	{
		temp = (double)temp * 0.625;
	}

	return temp;
}
