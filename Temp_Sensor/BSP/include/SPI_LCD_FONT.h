/**
 * @file     SPI_LCD_FONT.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-14
 * @brief    Font Support: 12*12,16*16,24*24,32*32 pixel ASCII character sets
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _SPI_LCD_FONT_H_
#define _SPI_LCD_FONT_H_

typedef enum
{
	Size_12_font = 12,
	Size_16_font = 16,
	Size_24_font = 24,
	Size_32_font = 32,
} Char_Size_t;

/* 常用ASCII表
 * 偏移量32
 * ASCII字符集: !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
 * PC2LCD2002取模方式设置：阴码+逐行式+顺向+C51格式
 * 总共：4个字符集（12*12、16*16、24*24和32*32），用户可以自行新增其他分辨率的字符集。
 * 每个字符所占用的字节数为:(size/8+((size%8)?1:0))*(size/2),其中size:是字库生成时的点阵大小(12/16/24/32...)
 */

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char asc2_2412[95][48];
extern const unsigned char asc2_3216[95][64];

#endif
