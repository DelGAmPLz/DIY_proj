
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "EXTI.h"
#include "stdlib.h"
#include "string.h"
#include "../BSP/include/IIC_EEPROM.h"

const static char *TAG = "I2C_EEPROM:";

/* test mesg */
#define TEST_MSG "hello,world!\n"
#define TEST_MSG_SIZE (sizeof(TEST_MSG))

void func_task(void *arg)
{

	TaskHandle_t handler = xTaskGetCurrentTaskHandle();
	EXTI_Init(handler);

	if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == 1)
	{
		EEPROM_Write_Page((uint8_t *)TEST_MSG, TEST_MSG_SIZE, 0);
		uint8_t *r_buf = malloc(TEST_MSG_SIZE);
		EEPROM_Read_Page(r_buf, TEST_MSG_SIZE, 0);
		vTaskDelay(100);
		ESP_LOGI("FUNC_TASK", "func_done!\n");
	}

	vTaskDelete(NULL);
}

void app_main(void)
{

	/* read and write eeprom */
	ESP_ERROR_CHECK(I2C_EEPROM_Init());

	xTaskCreate(func_task, "func_task", 4096, NULL, 10, NULL);
	ESP_LOGI(TAG, "task created!\n");

	while (1)
	{
	}

	/* int i = 0;
	while (i < TEST_MSG_SIZE)
	{
		printf("%c", r_buf[i++]);
	} */
}