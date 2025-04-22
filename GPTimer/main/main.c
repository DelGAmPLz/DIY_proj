/**
 * @file     main.c
 * @author   Andy King 杨啸
 * @date     Created on 2025-03-24
 * @brief    Test for gptimer API to control LED
 **************************************************************************************************
 * @attention
 * This project is programming for the ALIENTEK module based on the ESP32-S3 board.
 * As a beginner's project, it extends and modifies the source code provided by ALIENTEK,
 * intended solely for learning and reference purpose.
 **/

#include "../BSP/include/Gptimer.h"
#include "../BSP/include/LED.h"

void app_main(void)
{
	/* create a queue to pass info from ISR and main function */
	QueueHandle_t queue = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

	/* declare a srtuct to store counts info from ISR */
	gptimer_event_queue_element ele;

	Gptimer_Init(queue);
	LED_Init();

	while (1)
	{
		/* processing after data arrival */
		if (xQueueReceive(queue, &ele, portMAX_DELAY) == pdTRUE)
		{
			LED_Toggle();
			printf("LED toggled!\n");
		}
		else
		{
			printf("TIMER ERROR!\n");
		}
	}
}
