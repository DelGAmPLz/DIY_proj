/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-04-26
 * @brief    Test for API to take a picture
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "SPI_LCD.h"
#include "XL9555.h"
#include "UART1.h"
#include "esp_camera.h"
#include <stdio.h>

/* 引脚配置 */
#define CAM_PIN_PWDN GPIO_NUM_NC
#define CAM_PIN_RESET GPIO_NUM_NC
#define CAM_PIN_VSYNC GPIO_NUM_47
#define CAM_PIN_HREF GPIO_NUM_48
#define CAM_PIN_PCLK GPIO_NUM_45
#define CAM_PIN_XCLK GPIO_NUM_NC
#define CAM_PIN_SIOD GPIO_NUM_39
#define CAM_PIN_SIOC GPIO_NUM_38
#define CAM_PIN_D0 GPIO_NUM_4
#define CAM_PIN_D1 GPIO_NUM_5
#define CAM_PIN_D2 GPIO_NUM_6
#define CAM_PIN_D3 GPIO_NUM_7
#define CAM_PIN_D4 GPIO_NUM_15
#define CAM_PIN_D5 GPIO_NUM_16
#define CAM_PIN_D6 GPIO_NUM_17
#define CAM_PIN_D7 GPIO_NUM_18

#define CAM_PWDN(x)                                                              \
	do                                                                           \
	{                                                                            \
		x ? (XL9555_Write_IO(OV_PWDN_IO, 1)) : (XL9555_Write_IO(OV_PWDN_IO, 0)); \
	} while (0)

#define CAM_RST(x)                                                                 \
	do                                                                             \
	{                                                                              \
		x ? (XL9555_Write_IO(OV_RESET_IO, 1)) : (XL9555_Write_IO(OV_RESET_IO, 0)); \
	} while (0)

/* 摄像头配置 */
static camera_config_t camera_config = {
	/* 引脚配置 */
	.pin_pwdn = CAM_PIN_PWDN,
	.pin_reset = CAM_PIN_RESET,
	.pin_xclk = CAM_PIN_XCLK,
	.pin_sccb_sda = CAM_PIN_SIOD,
	.pin_sccb_scl = CAM_PIN_SIOC,

	.pin_d7 = CAM_PIN_D7,
	.pin_d6 = CAM_PIN_D6,
	.pin_d5 = CAM_PIN_D5,
	.pin_d4 = CAM_PIN_D4,
	.pin_d3 = CAM_PIN_D3,
	.pin_d2 = CAM_PIN_D2,
	.pin_d1 = CAM_PIN_D1,
	.pin_d0 = CAM_PIN_D0,
	.pin_vsync = CAM_PIN_VSYNC,
	.pin_href = CAM_PIN_HREF,
	.pin_pclk = CAM_PIN_PCLK,

	/* XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental) */
	.xclk_freq_hz = 24 * 1000 * 1000,
	.ledc_timer = LEDC_TIMER_0,
	.ledc_channel = LEDC_CHANNEL_0,

	.pixel_format = PIXFORMAT_RGB565, /* YUV422,GRAYSCALE,RGB565,JPEG */
	.frame_size = FRAMESIZE_240X240,  /* QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates */

	.jpeg_quality = 12, /* 0-63, for OV series camera sensors, lower number means higher quality */
	.fb_count = 2,		/* When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode */
	.fb_location = CAMERA_FB_IN_PSRAM,
	.grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

/**
 * @brief       摄像头初始化
 * @param       无
 * @retval      esp_err_t
 */
static esp_err_t init_camera(void)
{
	if (CAM_PIN_PWDN == GPIO_NUM_NC)
	{
		CAM_PWDN(0);
	}

	if (CAM_PIN_RESET == GPIO_NUM_NC)
	{
		CAM_RST(0);
		vTaskDelay(pdMS_TO_TICKS(20));
		CAM_RST(1);
		vTaskDelay(pdMS_TO_TICKS(20));
	}

	/* 摄像头初始化 */
	esp_err_t err = esp_camera_init(&camera_config);

	if (err != ESP_OK)
	{
		ESP_LOGE("TAG", "Camera Init Failed");
		return err;
	}

	sensor_t *s = esp_camera_sensor_get();

	/* 如果摄像头模块是OV3660或者是OV5640，则需要以下配置 */
	if (s->id.PID == OV3660_PID)
	{
		s->set_vflip(s, 1);		  /* 向后翻转 */
		s->set_brightness(s, 1);  /* 亮度提高 */
		s->set_saturation(s, -2); /* 降低饱和度 */
	}
	else if (s->id.PID == OV5640_PID)
	{
		s->set_vflip(s, 1); /* 向后翻转 */
	}

	return ESP_OK;
}
void app_main(void)
{
	esp_err_t ret;
	camera_fb_t *fb = NULL;

	usart_init(921600);
	SPI_LCD_Init();
	init_camera(); /* 初始化摄像头 */

	while (1)
	{
		fb = esp_camera_fb_get();
		uart_write_bytes(USART_UX, fb->buf, fb->len);
		esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, fb->width, fb->height, fb->buf);
		esp_camera_fb_return(fb);
	}
}