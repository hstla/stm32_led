// 폴링과 인터럽트 동작 비교(폴링 방식)

#include <stdbool.h>
#include "04_polling.h"
#include "main.h"

void led_polling_run()
{
	while(1) {
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		{
			HAL_GPIO_WritePin(EXT_GPIO_Port, EXT_LED_Pin, GPIO_PIN_SET);
			HAL_Delay(2000);
			HAL_GPIO_WritePin(EXT_GPIO_Port, EXT_LED_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		HAL_Delay(1000);
	}
}
