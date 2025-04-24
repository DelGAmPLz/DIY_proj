#ifndef _SPI_LCD_H_
#define _SPI_LCD_H_

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_io_spi.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_st7789.h"
#include "esp_lcd_panel_ops.h"
#include "XL9555.h"
#include "esp_heap_caps.h"
#include "SPI_LCD_FONT.h"

/* SPI_LCD PINS */
#ifndef SPI_LCD_PINS

#define LCD_SCLK_PIN GPIO_NUM_12
#define LCD_MOSI_PIN GPIO_NUM_11
#define LCD_MISO_PIN GPIO_NUM_13
#define LCD_DC_PIN GPIO_NUM_40
#define LCD_CS_PIN GPIO_NUM_21

#endif

/* SPI_LCD CONFIG */
#ifndef SPI_LCD_CONFIG

extern uint16_t spilcd_width;  /* 屏幕的宽度 240(横屏) */
extern uint16_t spilcd_height; /* 屏幕的宽度 240(横屏) */
#define LCD_PWR -1
#define LCD_RST -1
#define LCD_HOST SPI2_HOST
#define LCD_FREQ 20 * 1000 * 1000

#endif

/* SPI_LCD COLOR VALUE */
#ifndef SPI_LCD_COLOR
/* 常用颜色值 */
#define WHITE 0xFFFF   /* 白色 */
#define BLACK 0x0000   /* 黑色 */
#define RED 0xF800	   /* 红色 */
#define GREEN 0x07E0   /* 绿色 */
#define BLUE 0x001F	   /* 蓝色 */
#define MAGENTA 0XF81F /* 洋红色 */
#define YELLOW 0XFFE0  /* 黄色 */
#define CYAN 0X07FF	   /* 蓝绿色 */

/* 非常用颜色 */
#define BROWN 0XBC40	  /* 棕色 */
#define BRRED 0XFC07	  /* 棕红色 */
#define GRAY 0X8430		  /* 灰色 */
#define DARKBLUE 0X01CF	  /* 深蓝色 */
#define LIGHTBLUE 0X7D7C  /* 浅蓝色 */
#define GRAYBLUE 0X5458	  /* 灰蓝色 */
#define LIGHTGREEN 0X841F /* 浅绿色 */
#define LGRAY 0XC618	  /* 浅灰色(PANNEL),窗体背景色 */
#define LGRAYBLUE 0XA651  /* 浅灰蓝色(中间层颜色) */
#define LBBLUE 0X2B12	  /* 浅棕蓝色(选择条目的反色) */

#endif

/* SPI_LCD configuration */
typedef struct
{
	uint32_t pwidth;  /* 临时设定值（宽度） */
	uint32_t pheight; /* 临时设定值（高度） */
	uint8_t dir;	  /* 屏幕方向 */
	uint16_t width;	  /* 宽度 */
	uint16_t height;  /* 高度 */
	uint16_t background;
} SPI_LCD_cfg;

/* Declare LCD param  */
extern SPI_LCD_cfg spilcd_cfg;
extern esp_lcd_panel_handle_t panel_handle;

esp_err_t SPI_LCD_Init(void);

esp_err_t SPI_LCD_Display_Dir(uint8_t direction);

esp_err_t SPI_LCD_Background(uint16_t color);

esp_err_t SPI_LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);

esp_err_t SPI_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);

esp_err_t SPI_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

esp_err_t SPI_LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

esp_err_t SPI_LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

esp_err_t SPI_LCD_DrawHline(uint16_t x, uint16_t y, uint16_t len, uint16_t color);

esp_err_t SPI_LCD_ShowChar(uint16_t x, uint16_t y, uint8_t chr, Char_Size_t size, uint16_t char_color, uint16_t char_background_color);

uint32_t lcd_pow(uint8_t m, uint8_t n);

esp_err_t SPI_LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, Char_Size_t size, uint16_t color);

esp_err_t SPI_LCD_ShowXnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, Char_Size_t size, uint8_t mode, uint16_t color);

esp_err_t SPI_LCD_ShowString(uint16_t x, uint16_t y, uint16_t columns, uint16_t rows, Char_Size_t size, char *p, uint16_t char_color, uint16_t char_background_color);

#endif