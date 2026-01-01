/*
 * 신호등
 * 기능
 * 초록불 3초 노란불 1초 빨간불 2초
 *
 * 사용자 버튼 누를 시 야간 모드로 전환
 * 노란색 1초 점멸 3초 후 다시 메인모드로 전환
 */

#include <stdint.h>
#include <stdbool.h>
#include "07_traffic_light.h"
#include "00_timer2.h"
#include "main.h"

static bool night_flag = false;

typedef enum {
	STATE_DAY_GREEN = 0,
	STATE_DAY_YELLOW,
	STATE_DAY_RED,
	STATE_NIGIT_BLINK
} traffic_state;

static void set_leds(uint8_t green, uint8_t yellow, uint8_t red) {
	HAL_GPIO_WritePin(LED_PORT, GREEN_LED_PIN, green ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, YELLOW_LED_PIN, yellow ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_PORT, RED_LED_PIN, red ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void traffic_light_run(void)
{

	traffic_state state = STATE_DAY_GREEN;
	uint32_t state_start_ms = get_tim2_ms();
	uint32_t last_blink_ms = 0;
	uint32_t blink_count = 0;

	set_leds(1, 0, 0); // 초록불 시작

	while(1)
	{
		uint32_t now = get_tim2_ms();

		if(night_flag)
		{
			night_flag = false;
			state = STATE_NIGIT_BLINK;
			state_start_ms = now;
			last_blink_ms = now;
			blink_count = 0;
			set_leds(0, 1, 0);
		}

		switch (state)
		{
		case STATE_DAY_GREEN:
			if (now - state_start_ms >= 3000)
			{
				state = STATE_DAY_YELLOW;
				state_start_ms = now;
				set_leds(0, 1, 0);
			}
			break;

		case STATE_DAY_YELLOW:
			if(now - state_start_ms >= 1000)
			{
				state = STATE_DAY_RED;
				state_start_ms = now;
				set_leds(0, 0, 1);
			}
			break;

		case STATE_DAY_RED:
			if (now - state_start_ms >= 2000)
			{
				state = STATE_DAY_GREEN;
				state_start_ms = now;
				set_leds(1, 0, 0);
			}
			break;

		case STATE_NIGIT_BLINK:
			// 노란색 1초마다 점멸
			if (now - last_blink_ms >= 1000)
			{
				last_blink_ms = now;
				HAL_GPIO_TogglePin(LED_PORT, YELLOW_LED_PIN);
				blink_count++;
			}

			if(blink_count >= 6)
			{
				state = STATE_DAY_GREEN;
				state_start_ms = now;
				set_leds(1, 0, 0);
				blink_count = 0;
			}
			break;

		default:
			state = STATE_DAY_GREEN;
			state_start_ms = now;
			set_leds(1, 0, 0);
			break;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t last_exti_time = 0;
	uint32_t current_time = get_tim2_ms();

  if (GPIO_Pin == B1_Pin)
  {
  	if (current_time - last_exti_time > 200)
  	{
  		night_flag = true;
  		last_exti_time = current_time;
  	}
  }
}
