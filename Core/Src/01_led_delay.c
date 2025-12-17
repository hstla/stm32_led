#include "01_led_delay.h"
#include "main.h"

void led_delay_run(void)
{
  while (1)
  {
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    HAL_Delay(500);
  }
}
