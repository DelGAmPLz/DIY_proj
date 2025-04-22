#include "../BSP/include/XL9555.h"

void app_main(void)
{
	XL9555_Init();
	while (1)
	{

		/* vTaskDelay(1000);
		test();
		// XL9555_IO_Mode(0xF003);
		test_();
		vTaskDelay(1000);
		test(); */

		if (XL9555_Read_IO(KEY3_IO) == 0)
		{
			printf("key is pressed!\n");
			vTaskDelay(10);
		}
	}
}