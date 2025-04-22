#include "esp_err.h"
#include "esp_log.h"
#include "../BSP/include/ADC.h"
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

adc_oneshot_unit_handle_t adc_oneshot_handle = NULL;

/**
 *
 *
 * @brief      Initialize ADC oneshot mode
 *             1.adc_oneshot_unit_init_cfg_t
 *             2.adc_oneshot_chan_cfg_t
 * @param      void
 * @return     esp_err_t
 *
 *
 **/

esp_err_t ADC_Init(void)
{

	adc_oneshot_unit_init_cfg_t adc_onshot_cfg = {
		.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
		.ulp_mode = ADC_ULP_MODE_DISABLE,
		.unit_id = ADC_UNIT_1,
	};

	ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc_onshot_cfg, &adc_oneshot_handle));

	adc_oneshot_chan_cfg_t adc_oneshot_chan_cfg = {
		.atten = ADC_ATTEN_DB_12,
		.bitwidth = ADC_BITWIDTH_DEFAULT,
	};

	ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_oneshot_handle, ADC_CHANNEL, &adc_oneshot_chan_cfg));
	return ESP_OK;
}

/**
 *
 *
 * @brief      Head-tail trimmed average output
 * @param      adc_channel_t channel, int times
 * @return     uint32_t
 *
 *
 **/

#define LOST_VAL 1

uint32_t ADC_GetResult(adc_channel_t channel, int times)
{
	uint32_t sum = 0;
	uint16_t temp_val = 0;

	/* 申请存放ADC原始数据buffer */
	int *rawdata = heap_caps_malloc(times * sizeof(int), MALLOC_CAP_INTERNAL);
	if (NULL == rawdata)
	{
		ESP_LOGE("adc", "Memory for adc is not enough");
	}

	for (uint32_t t = 0; t < times; t++) /* 多次ADC采样 */
	{
		adc_oneshot_read(adc_oneshot_handle, channel, &rawdata[t]); /* 读取原始数据 */
		vTaskDelay(5);
	}

	for (uint16_t i = 0; i < times - 1; i++) /* 对数据进行排序 */
	{
		for (uint16_t j = i + 1; j < times; j++)
		{
			if (rawdata[i] > rawdata[j]) /* 升序排列 */
			{
				temp_val = rawdata[i];
				rawdata[i] = rawdata[j];
				rawdata[j] = temp_val;
			}
		}
	}

	for (uint32_t i = LOST_VAL; i < times - LOST_VAL; i++) /* 去掉两端的丢弃值 */
	{
		sum += rawdata[i]; /* 累加去掉丢弃值以后的数据 */
	}

	return sum / (times - 2 * LOST_VAL); /* 返回平均值 */
}

/* ********************************************* unused **************************************************** */

/* kalman fliter */
kalman_filter_t kalman_cfg;

bool Init_flag = false;

void Kalman_Init(kalman_filter_t *kf, float q, float r)
{
	kf->q = q;
	kf->r = r;
	kf->p = 1000.0f; // 初始估计误差
	kf->k = 0.0f;
	kf->x = 0.0f;
}

float Kalman_Get(float q, float r, float measurement)
{
	if (!Init_flag)
	{
		Kalman_Init(&kalman_cfg, q, r);
		Init_flag = true;
	}

	kalman_cfg.p += kalman_cfg.q;

	// 更新阶段
	kalman_cfg.k = kalman_cfg.p / (kalman_cfg.p + kalman_cfg.r);
	kalman_cfg.x = kalman_cfg.x + kalman_cfg.k * (measurement - kalman_cfg.x);
	kalman_cfg.p = (1.0f - kalman_cfg.k) * kalman_cfg.p;

	return kalman_cfg.x;
}