#include "03_led_pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;

void led_pwm_run(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  while (1)
  {
    for (uint32_t duty = 0; duty <= htim2.Init.Period; duty += 50)
    {
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
      HAL_Delay(10);
    }

    for (int32_t duty = htim2.Init.Period; duty >= 0; duty -= 50)
    {
      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
      HAL_Delay(10);
    }
  }
}
