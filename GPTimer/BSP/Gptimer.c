/**
 * @file     GPTimer.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-25
 * @brief    definition of gptimer API to initialize gptimer and create ISR event
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/GPTimer.h"

/**
 *
 *
 * @brief      1.initialize gptimer_handle_t and gptimer_config_t,config gptimer_config_t struct
 * 			   2.create new gptimer
 *			   3.config and register event callback function
 *			   (using param_gptimer_event_queue to pass context info to callback_function)
 *			   4.enable gptimer
 *			   5.config alarm_event_st and define alarm event
 *			   6.start gptimer
 * @param      QueueHandle_t gptimer_event_queue pass context info to callback_function
 * @return     void
 *
 *
 **/

void Gptimer_Init(QueueHandle_t gptimer_event_queue)
{
	gptimer_handle_t gptimer_handler = NULL;

	gptimer_config_t gptimer_cfg = {
		.clk_src = GPTIMER_CLK_SRC_DEFAULT,
		.direction = GPTIMER_COUNT_UP,
		.resolution_hz = 1000000,
		.intr_priority = 0,
		.flags.intr_shared = 0,
	};

	gptimer_new_timer(&gptimer_cfg, &gptimer_handler);

	gptimer_event_callbacks_t Cb_St = {
		.on_alarm = Gptimer_Event_Cb,
	};

	gptimer_register_event_callbacks(gptimer_handler, &Cb_St, gptimer_event_queue);

	gptimer_enable(gptimer_handler);

	gptimer_alarm_config_t alarm_cfg = {
		.alarm_count = 1000000,
		.reload_count = 0,
		.flags.auto_reload_on_alarm = 1,
	};

	gptimer_set_alarm_action(gptimer_handler, &alarm_cfg);

	gptimer_start(gptimer_handler);
}

/**
 *
 *
 * @brief      definition of callback_function
 *			   configuration steps bellow
 *			   1.define high_task_awoken for xQueueSendFromISR and cast type for param: user_ctx
 *			   2.declare a gptimer_event_queue_element variable and assigna value for counts
 *			   3.send context(ele) to queue from ISR
 *			   4.return value to awaken zhen function which has higher priority than the blocked function
 * @param      1)gptimer_handle_t timer:timer handle(config gptimer)
 *			   2)const gptimer_alarm_event_data_t *edata:context information(counts and alarm value when jumping to ISR)
 * 			   3)void *user_ctx:queue which stores ISR context info to send
 * @return     whether a context switch needed when returning from ISR
 *
 *
 **/

static bool IRAM_ATTR Gptimer_Event_Cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{

	BaseType_t high_task_awoken = pdFALSE;

	QueueHandle_t queue = (QueueHandle_t)user_ctx;

	gptimer_event_queue_element ele = {.counts = edata->count_value};

	xQueueSendFromISR(queue, &ele, &high_task_awoken);

	return high_task_awoken == pdTRUE;
}