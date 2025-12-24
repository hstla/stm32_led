#include "main.h"
#include "00_timer2.h"
#include "02_led_timer.h"

static uint32_t led_counter = 0;

// 1ms 타이머를 사용하여 0.5초동안 깜빡이는 동작 구현
static void led_blink_interrupt(void)
{
  led_counter++;

  if (led_counter >= 500) {
  	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  	led_counter = 0;
  }
}

void led_timer_run(void)
{
	tim2_register_callback(led_blink_interrupt);
}
