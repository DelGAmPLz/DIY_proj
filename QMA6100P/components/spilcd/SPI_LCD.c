/**
 * @file     SPI_LCD.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-14
 * @brief    Initialize API for SPI_LCD
 *          (The API in this document is designed for 1.3-inch LCD displays with SPI interface. )
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/SPI_LCD.h"
#include "string.h"

/* Initialize struct to store LCD configutation */
SPI_LCD_cfg spilcd_cfg;

/* ESP_LCD panel interface handler */
esp_lcd_panel_handle_t panel_handle = NULL;

/*  */
uint16_t spilcd_width = 240;
uint16_t spilcd_height = 240;

/**
 *
 *
 * @brief      Initialize SPI:
 *             1.spi_bus_config_t
 *
 *             Initialize PANEL:
 *             2.esp_lcd_panel_io_spi_config
 *             3.esp_lcd_panel_dev_config_t
 *
 *             Start LCD:
 *             4.esp_lcd_new_panel_st7789 -- start st7789 driver
 *             5.reset panel->
 *               init panel->
 *               LCD display on->
 *               invert color->
 *               set direction
 *               set background solor
 * @param      void
 * @return     return esp_err_t
 *
 *
 **/

esp_err_t SPI_LCD_Init(void)
{
	esp_err_t err = XL9555_Init();
	if (err != ESP_OK)
		return err;

	/*
	   create a SPI bus
	   choosing the octal mode
	*/
	spi_bus_config_t spi_cfg = {
		.sclk_io_num = LCD_SCLK_PIN,
		.miso_io_num = LCD_MISO_PIN,
		.mosi_io_num = LCD_MOSI_PIN,
		.quadhd_io_num = -1,
		.quadwp_io_num = -1,
		.max_transfer_sz = spilcd_width * spilcd_height * sizeof(uint16_t),
		/* if need ,set flags for SPI mode */
	};

	spi_host_device_t spi_dev_cfg = LCD_HOST;

	/* initialize spi bus */
	spi_bus_initialize(LCD_HOST, &spi_cfg, SPI_DMA_CH_AUTO);

	esp_lcd_panel_io_handle_t io_handle = NULL;

	/* create a IO panel handle from SPI */
	esp_lcd_panel_io_spi_config_t panal_cfg = {
		.dc_gpio_num = LCD_DC_PIN,
		.cs_gpio_num = LCD_CS_PIN,
		.pclk_hz = LCD_FREQ,
		.spi_mode = 0, /* 0 for single line ,1 for double lines...3 for octal lines */

		.lcd_cmd_bits = 8,
		.lcd_param_bits = 8,
		/* ​​Command and Parameter Bit Width Supported by the LCD Controller​​
		   Which means the frame accept 8 bit length
		*/

		.trans_queue_depth = 7,
		/* each transaction task costs 48~64 bytes,the deeper queue depth is,the more memory costs */
	};

	ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &panal_cfg, &io_handle));

	/* initialize LCD controler driver */
	esp_lcd_panel_dev_config_t dev_cfg = {
		.reset_gpio_num = LCD_RST,
		.rgb_ele_order = COLOR_RGB_ELEMENT_ORDER_RGB, /* color data RGB order */
		.bits_per_pixel = 16,						  /* color data bit */
		.rgb_endian = LCD_RGB_DATA_ENDIAN_BIG,		  /* data endian for those size over 1 byte */
	};

	/* create panel handle for st7789 driver */
	ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &dev_cfg, &panel_handle));

	/* reset LCD before initialization */
	ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));

	/* init */
	ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

	/* turn on LCD display different from the back light */
	ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

	/* invert color */
	ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

	/* set direction for display : landscape display/portrait display */
	ESP_ERROR_CHECK(SPI_LCD_Display_Dir(1));

	/* set LCD bg color */
	ESP_ERROR_CHECK(SPI_LCD_Background(WHITE));
	printf("initialized!\n");

	return ESP_OK;
}

/**
 *
 *
 * @brief      set display direction(​​Default orientation is portrait)
 * @param      uint8_t direction : 1 for landscape display, 0 for portrait display
 * @return     esp_err_t
 *
 * @attention  Can be rotated to other orientations for display.​
 **/

esp_err_t SPI_LCD_Display_Dir(uint8_t direction)
{
	if (direction != 0 && direction != 1)
	{
		return ESP_ERR_INVALID_ARG;
	}

	spilcd_cfg.dir = direction;
	if (spilcd_cfg.dir == 0)
	{
		spilcd_cfg.height = spilcd_width;
		spilcd_cfg.width = spilcd_height;

		ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
		ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
	}
	else
	{
		spilcd_cfg.height = spilcd_height;
		spilcd_cfg.width = spilcd_width;

		ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
		ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
		/* x axis for the Top edge of screen
		   y axis for the Left edge of screen
		*/
	}
	return ESP_OK;
}

/**
 *
 *
 * @brief      set LCD bg color
 * @param      uint16_t color which is required to swap endianness
 * @return     esp_err_t
 *
 *
 **/

esp_err_t SPI_LCD_Background(uint16_t color)
{
	spilcd_cfg.background = color;

	uint16_t color_tmp = ((color & 0x00FF) << 8 | (color & 0xFF00) >> 8);

	uint16_t *buffer = heap_caps_malloc(40 * spilcd_cfg.width * sizeof(uint16_t), MALLOC_CAP_DMA);
	if (!buffer)
		ESP_LOGE("LCD clear", "buffer malloc failed");
	else
	{
		for (uint32_t i = 0; i < 40 * spilcd_cfg.width; i++)
		{
			buffer[i] = color_tmp;
		}
		for (uint16_t y = 0; y < spilcd_cfg.height; y += 40)
		{
			ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, 0, y, spilcd_cfg.width, y + 40, buffer));
		}
	}
	heap_caps_free(buffer);
	return ESP_OK;
}

/**
 *
 *
 * @brief      ​​Check if input pixel coordinates are within the LCD visible area
 * @param      uint16_t x, uint16_t y
 * @return     bool
 *
 *
 **/

bool Check_Edge(uint16_t x, uint16_t y)
{
	if (x > spilcd_cfg.width)
		return false;
	if (y > spilcd_cfg.height)
		return false;
	return true;
}

/**
 *
 *
 * @brief      Fill a region
 * @param      uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color
 * @return     esp_err_t
 *
 *
 **/

esp_err_t SPI_LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
	if (!Check_Edge(sx, sy) || Check_Edge(ex, ey))
		return ESP_ERR_INVALID_ARG;

	uint16_t width = sx - ex + 1;
	uint16_t height = sy - ey + 1;

	uint16_t color_tmp = ((color & 0x00FF) << 8 | (color & 0xFF00) >> 8);

	uint16_t *buffer = heap_caps_malloc(width * sizeof(uint16_t), MALLOC_CAP_DMA);
	if (!buffer)
		ESP_LOGE("LCD clear", "buffer malloc failed");
	else
	{
		for (uint32_t i = 0; i < width; i++)
		{
			buffer[i] = color_tmp;
		}
		for (uint16_t y = sy; y < height; y++)
		{
			ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, sx, y, ex + 1, y + 1, buffer));
		}
	}
	heap_caps_free(buffer);
	return ESP_OK;
}

esp_err_t SPI_LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	if (!Check_Edge(x, y))
		return ESP_ERR_INVALID_ARG;
	uint16_t color_tmp = ((color & 0x00FF) << 8 | (color & 0xFF00) >> 8);
	ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + 1, y + 1, &color_tmp));
	return ESP_OK;
}

esp_err_t SPI_LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	if (!Check_Edge(x1, y1) || !Check_Edge(x2, y2))
		return ESP_ERR_INVALID_ARG;
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, row, col;
	delta_x = x2 - x1;
	delta_y = y2 - y1;
	row = x1;
	col = y1;

	if (delta_x > 0)
	{
		incx = 1;
	}
	else if (delta_x == 0)
	{
		incx = 0;
	}
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	if (delta_y > 0)
	{
		incy = 1;
	}
	else if (delta_y == 0)
	{
		incy = 0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}

	if (delta_x > delta_y)
	{
		distance = delta_x;
	}
	else
	{
		distance = delta_y;
	}

	for (t = 0; t <= distance + 1; t++)
	{
		SPI_LCD_DrawPoint(row, col, color);
		xerr += delta_x;
		yerr += delta_y;

		if (xerr > distance)
		{
			xerr -= distance;
			row += incx;
		}

		if (yerr > distance)
		{
			yerr -= distance;
			col += incy;
		}
	}
	return ESP_OK;
}

esp_err_t SPI_LCD_DrawHline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
	if ((len == 0) || (x > spilcd_cfg.width) || (y > spilcd_cfg.height))
		return ESP_ERR_INVALID_ARG;
	if (!Check_Edge(x, y))
		return ESP_ERR_INVALID_ARG;

	SPI_LCD_Fill(x, y, x + len - 1, y, color);
	return ESP_OK;
}

esp_err_t SPI_LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	if (!Check_Edge(x1, y1) || !Check_Edge(x0, y0))
		return ESP_ERR_INVALID_ARG;
	SPI_LCD_DrawLine(x0, y0, x1, y0, color);
	SPI_LCD_DrawLine(x0, y0, x0, y1, color);
	SPI_LCD_DrawLine(x0, y1, x1, y1, color);
	SPI_LCD_DrawLine(x1, y0, x1, y1, color);
	return ESP_OK;
}

esp_err_t SPI_LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
	if (!Check_Edge(x0, y0))
		return ESP_ERR_INVALID_ARG;
	int a, b;
	int di;
	a = 0;
	b = r;
	di = 3 - (r << 1);

	while (a <= b)
	{
		SPI_LCD_DrawPoint(x0 - b, y0 - a, color);
		SPI_LCD_DrawPoint(x0 + b, y0 - a, color);
		SPI_LCD_DrawPoint(x0 - a, y0 + b, color);
		SPI_LCD_DrawPoint(x0 - b, y0 - a, color);
		SPI_LCD_DrawPoint(x0 - a, y0 - b, color);
		SPI_LCD_DrawPoint(x0 + b, y0 + a, color);
		SPI_LCD_DrawPoint(x0 + a, y0 - b, color);
		SPI_LCD_DrawPoint(x0 + a, y0 + b, color);
		SPI_LCD_DrawPoint(x0 - b, y0 + a, color);
		a++;

		if (di < 0)
		{
			di += 4 * a + 6;
		}
		else
		{
			di += 10 + 4 * (a - b);
			b--;
		}

		SPI_LCD_DrawPoint(x0 + a, y0 + b, color);
	}
	return ESP_OK;
}

/**
 *
 *
 * @brief      render text using font library with specified color
 * @param      uint16_t x, uint16_t y, uint8_t chr, Char_Size_t size,
 *             uint16_t char_color, uint16_t char_background_color
 * @return     esp_err_t
 *
 *
 **/

esp_err_t SPI_LCD_ShowChar(uint16_t x, uint16_t y, uint8_t chr, Char_Size_t size, uint16_t char_color, uint16_t char_background_color)
{
	if (!Check_Edge(x, y))
		return ESP_ERR_INVALID_ARG;
	const uint8_t *ch_code; /* 存放chr字符对应数组的首地址 */
	uint8_t ch_width;		/* 字符的宽度 */
	uint8_t ch_height;		/* 字符的高度 */
	uint8_t ch_size;		/* 字符的大小(字节) */
	uint8_t ch_offset;		/* 字符在字库的相对位置 */
	uint8_t byte_index;		/* 字符对应数据的索引值 */
	uint8_t byte_code;		/* 字符对应数据 */
	uint8_t bit_index;		/* 字符对应字节数据的位索引 */
	uint16_t colortemp = 0; /* 颜色数据 */
	uint16_t pix_index = 0;
	uint16_t *pcolor = NULL;

	/* 字体大小(字节) =       字体宽度占用字体大小              * 字体高度 */
	ch_size = ((size / 2) / 8 + (((size / 2) % 8) ? 1 : 0)) * size;													   /* 得到字体一个字符对应点阵集所占的字节数 */
	uint16_t color_tmp = ((char_color & 0x00FF) << 8) | ((char_color & 0xFF00) >> 8);								   /* 需要转换一下颜色值 */
	uint16_t background_color_tmp = ((char_background_color & 0x00FF) << 8) | ((char_background_color & 0xFF00) >> 8); /* 需要转换一下颜色值 */

	ch_offset = chr - ' '; /* 得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库） */

	switch (size)
	{
	case 12:
		ch_code = (uint8_t *)asc2_1206[ch_offset]; /* 调用1206字体 */
		ch_width = 6;
		ch_height = 12;
		break;

	case 16:
		ch_code = (uint8_t *)asc2_1608[ch_offset]; /* 调用1608字体 */
		ch_width = 8;
		ch_height = 16;
		break;

	case 24:
		ch_code = (uint8_t *)asc2_2412[ch_offset]; /* 调用2412字体 */
		ch_width = 12;
		ch_height = 24;
		break;

	case 32:
		ch_code = (uint8_t *)asc2_3216[ch_offset]; /* 调用3216字体 */
		ch_width = 16;
		ch_height = 32;
		break;

	default:
		return ESP_ERR_INVALID_ARG;
	}

	pcolor = heap_caps_malloc((ch_width * ch_height * 2) + 1, MALLOC_CAP_INTERNAL); /* 申请大小 */
	if (NULL == pcolor)
	{
		ESP_LOGE("TAG", "Memory for bitmap is not enough");
	}

	for (byte_index = 0; byte_index < ch_size; byte_index++)
	{
		byte_code = ch_code[byte_index]; /* 获取字符的点阵数据 */

		for (bit_index = 0; bit_index < 8; bit_index++) /* 一个字节8个点 */
		{
			if ((byte_code & 0x80) != 0) /* 有效点,需要显示 */
			{
				colortemp = color_tmp;
			}
			else
				colortemp = background_color_tmp;

			pcolor[pix_index] = colortemp;
			pix_index++;

			if ((size == 24) && (byte_index % 2)) /* 24号字体比较特殊,奇数字节只有四位有效 */
			{
				if (bit_index == 3)
				{
					break;
				}
			}

			byte_code <<= 1; /* 移位, 以便获取下一个位的状态 */
		}
	}

	ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel_handle, x, y, x + ch_width, y + ch_height, (uint16_t *)pcolor));

	heap_caps_free(pcolor);
	vTaskDelay(1);
	return ESP_OK;
}

/* calculate number */
uint32_t lcd_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;

	while (n--)
		result *= m;

	return result;
}

esp_err_t SPI_LCD_ShowNum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, Char_Size_t size, uint16_t color)
{
	if (!Check_Edge(x, y))
		return ESP_ERR_INVALID_ARG;
	uint16_t color_tmp = ((color & 0x00FF) << 8) | ((color & 0xFF00) >> 8);
	uint16_t bg_color = spilcd_cfg.background;
	uint8_t t, temp;
	uint8_t enshow = 0;

	for (t = 0; t < len; t++) /* 按总显示位数循环 */
	{
		temp = (num / lcd_pow(10, len - t - 1)) % 10; /* 获取对应位的数字 */

		if (enshow == 0 && t < (len - 1)) /* 没有使能显示,且还有位要显示 */
		{
			if (temp == 0)
			{
				SPI_LCD_ShowChar(x + (size / 2) * t, y, ' ', size, color_tmp, bg_color); /* 显示空格,占位 */
				continue;																 /* 继续下个一位 */
			}
			else
			{
				enshow = 1; /* 使能显示 */
			}
		}

		SPI_LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, color_tmp, bg_color); /* 显示字符 */
	}
	return ESP_OK;
}

esp_err_t SPI_LCD_ShowXnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, Char_Size_t size, uint8_t mode, uint16_t color)
{
	if (!Check_Edge(x, y))
		return ESP_ERR_INVALID_ARG;
	uint8_t t, temp;
	uint8_t enshow = 0;
	uint16_t color_tmp = ((color & 0x00FF) << 8) | ((color & 0xFF00) >> 8);
	uint16_t bg_color = spilcd_cfg.background;

	for (t = 0; t < len; t++) /* 按总显示位数循环 */
	{
		temp = (num / lcd_pow(10, len - t - 1)) % 10; /* 获取对应位的数字 */

		if (enshow == 0 && t < (len - 1)) /* 没有使能显示,且还有位要显示 */
		{
			if (temp == 0)
			{
				if (mode & 0X80) /* 高位需要填充0 */
				{
					SPI_LCD_ShowChar(x + (size / 2) * t, y, '0', size, color_tmp, bg_color); /* 用0占位 */
				}
				else
				{
					SPI_LCD_ShowChar(x + (size / 2) * t, y, ' ', size, color_tmp, bg_color); /* 用空格占位 */
				}
				continue;
			}
			else
			{
				enshow = 1; /* 使能显示 */
			}
		}
		SPI_LCD_ShowChar(x + (size / 2) * t, y, temp + '0', size, color, bg_color);
	}
	return ESP_OK;
}

/**
 *
 *
 * @brief      render string with specified color
 * @param      uint16_t x, uint16_t y, uint16_t columns, uint16_t rows,
 *             Char_Size_t size, char *p, uint16_t char_color, uint16_t char_background_color
 * @return     esp_err_t
 *
 *
 **/

esp_err_t SPI_LCD_ShowString(uint16_t x, uint16_t y, uint16_t columns, uint16_t rows, Char_Size_t size, char *p, uint16_t char_color, uint16_t char_background_color)
{

	if (!Check_Edge(x, y) || !Check_Edge(x + columns, y + rows))
		return ESP_ERR_INVALID_ARG;

	if ((columns * size / 2) > spilcd_cfg.width)
	{
		ESP_LOGE("ShowString:", "columns should be less than %d\n", 240 / (columns * size / 2));
		return ESP_ERR_INVALID_ARG;
	}
	if (rows * size > spilcd_cfg.height)
	{
		ESP_LOGE("ShowString:", "rows should be less than %d\n", 240 / (rows * size));
		return ESP_ERR_INVALID_ARG;
	}
	uint8_t x0 = x;
	columns = x + size / 2 * columns;
	rows = y + size * rows;

	while ((*p <= '~') && (*p >= ' ')) /* 判断是不是非法字符! */
	{
		if (x >= columns)
		{
			x = x0;
			y += size;
		}

		if (y >= rows)
			break; /* 退出 */

		SPI_LCD_ShowChar(x, y, *p, size, char_color, char_background_color);
		x += size / 2;
		p++;
	}
	return ESP_OK;
}