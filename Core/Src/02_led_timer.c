#include "02_led_timer.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;
volatile uint32_t tim2_ms_counter = 0;

void led_timer_run(void)
{
  HAL_TIM_Base_Start_IT(&htim2);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    tim2_ms_counter++;
    if (tim2_ms_counter >= 500)
    {
      tim2_ms_counter = 0;
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    }
  }
}
