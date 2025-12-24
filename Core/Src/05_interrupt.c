// 폴링과 인터럽트 동작 비교(인터럽트 방식)

#include <stdbool.h>
#include "00_timer2.h"
#include "05_interrupt.h"
#include "main.h"

static volatile bool ext_led_active = false;
static volatile uint32_t ext_led_end = 0;

static void ext_led_task(void);
static void internal_led_task(void);


void led_interrupt_run(void)
{
	tim2_register_callback(internal_led_task);
	tim2_register_callback(ext_led_task);
}


// 버튼 EXTI 콜백
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == B1_Pin)
  {
  	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  	HAL_GPIO_WritePin(EXT_GPIO_Port, EXT_LED_Pin, GPIO_PIN_SET);

  	ext_led_active = true;
    ext_led_end = get_tim2_ms() + 2000;
  }
}

static void ext_led_task(void)
{
	if (!ext_led_active) {
		return;
	}

	if ((int32_t)(get_tim2_ms() - ext_led_end) >= 0)
	{
		HAL_GPIO_WritePin(EXT_GPIO_Port, EXT_LED_Pin, GPIO_PIN_RESET);
		ext_led_active = false;
	}
}

static void internal_led_task(void)
{
	static uint32_t next_toggle = 0;
	static bool led_on = false;

	if (ext_led_active) {
			return;
	}

	if ((int32_t)(get_tim2_ms() - next_toggle) >= 0)
	{
		led_on = !led_on;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, led_on ? GPIO_PIN_SET : GPIO_PIN_RESET);

		next_toggle = get_tim2_ms() + (led_on ? 2000 : 1000);
	}
}
