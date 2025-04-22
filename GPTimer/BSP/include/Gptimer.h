
/**
 * @file     BSP.h
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-24
 * @brief    API for GPtimer to complete some simple functions
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#ifndef _GPTIMER_H_
#define _GPTIMER_H_

#include "driver/gptimer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

/* global variable to store timer counts from context information */
typedef struct
{
	uint64_t counts;
} gptimer_event_queue_element;

/* param of xqueuecreate() which describes how many elements the queue cam store at once */
#define QUEUE_LENGTH 10

/* param of xqueuecreate() which describes the amount of memory allocated for an element */
#define QUEUE_ITEM_SIZE sizeof(gptimer_event_queue_element)

void Gptimer_Init(QueueHandle_t gptimer_event_queue);

static bool IRAM_ATTR Gptimer_Event_Cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);

#endif
