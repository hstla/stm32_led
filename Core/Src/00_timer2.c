#include "main.h"
#include "00_timer2.h"

// TIM2에 들어가야하는 기능 수.
#define MAX_CALLBACK 3

volatile uint32_t tim2_tick_ms = 0;
extern TIM_HandleTypeDef htim2;

typedef void (*timer_cb_t)(void);
static timer_cb_t tim2_callbacks[MAX_CALLBACK];
static size_t tim2_cb_count = 0;

void timer2_run(void)
{
  HAL_TIM_Base_Start_IT(&htim2);

}

void tim2_register_callback(timer_cb_t cb)
{
	if (tim2_cb_count < MAX_CALLBACK)
	{
		tim2_callbacks[tim2_cb_count++] = cb;
	}
}

void tim2_unregister_all(void)
{
	tim2_cb_count = 0;
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
  	tim2_tick_ms++;

  	for (size_t i = 0; i < tim2_cb_count; ++i) {
  		tim2_callbacks[i]();
  	}
  }
}


uint32_t get_tim2_ms(void)
{
	return tim2_tick_ms;
}
