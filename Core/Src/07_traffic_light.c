#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "07_traffic_light.h"
#include "00_timer2.h"
#include "main.h"
#include "tm1637.h"

#define GREEN_MS 3000
#define YELLOW_MS 1000
#define RED_MS 2000

extern tm1637_t seg;

typedef enum {
    MODE_DAY = 0,
    MODE_NIGHT
} traffic_mode_t;

typedef enum {
    DAY_GREEN = 0,
    DAY_YELLOW,
    DAY_RED
} day_state_t;


static traffic_mode_t mode = MODE_DAY;
static day_state_t day_state = DAY_GREEN;

static volatile bool night_request = false;
static bool night_digit_display = false;

static uint32_t state_start_ms = 0;
static uint32_t last_blink_ms = 0;
static uint8_t blink_count = 0;

static void set_leds(uint8_t green, uint8_t yellow, uint8_t red)
{
    HAL_GPIO_WritePin(LED_PORT, GREEN_LED_PIN,
                      green ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, YELLOW_LED_PIN,
                      yellow ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_PORT, RED_LED_PIN,
                      red ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void display_countdown(uint32_t remaining_ms)
{
    char buf[4];
    uint32_t deciseconds = remaining_ms / 100;
    uint32_t seconds     = deciseconds / 10;
    uint32_t deci        = deciseconds % 10;

    sprintf(buf, "%02lu.%1lu", seconds, deci);

    if (buf[0] == '0')
    {
    	buf[0] = ' ';
    }
    tm1637_str(&seg, buf);
}

static void day_fsm_run(uint32_t now)
{
	uint32_t elapsed_ms = now - state_start_ms;
	uint32_t remaining_ms;

	switch (day_state)
	{
	case DAY_GREEN:
		remaining_ms = (elapsed_ms < GREEN_MS) ? (GREEN_MS - elapsed_ms) : 0;
		display_countdown(remaining_ms);

			if (now - state_start_ms >= GREEN_MS)
			{
					day_state = DAY_YELLOW;
					state_start_ms = now;
					set_leds(0, 1, 0);
			}
			break;

	case DAY_YELLOW:
		remaining_ms = (elapsed_ms < YELLOW_MS) ? (YELLOW_MS - elapsed_ms) : 0;
		display_countdown(remaining_ms);

		if (now - state_start_ms >= YELLOW_MS)
		{
				day_state = DAY_RED;
				state_start_ms = now;
				set_leds(0, 0, 1);
		}
		break;

	case DAY_RED:
		remaining_ms = (elapsed_ms < RED_MS) ? (RED_MS - elapsed_ms) : 0;
		display_countdown(remaining_ms);

		if (now - state_start_ms >= RED_MS)
		{
				day_state = DAY_GREEN;
				state_start_ms = now;
				set_leds(1, 0, 0);
		}
		break;
	}
}

static void night_fsm_run(uint32_t now)
{
	if (now - last_blink_ms >= 1000)
	{
		last_blink_ms = now;
		blink_count++;

		if (blink_count >= 6)
		{
				return;
		}

		HAL_GPIO_TogglePin(LED_PORT, YELLOW_LED_PIN);

		if (night_digit_display)
		{
			night_digit_display = false;
			tm1637_clear(&seg);
		}
		else
		{
			night_digit_display = true;
			tm1637_str(&seg, "88.88");
		}
	}

	if (blink_count >= 6)
	{
		mode = MODE_DAY;
		day_state = DAY_GREEN;
		state_start_ms = now;
		set_leds(1, 0, 0);
		tm1637_clear(&seg);
		night_digit_display = false;
		return;
	}
}

void traffic_light_run(void)
{
	uint32_t now;

	set_leds(1, 0, 0);
	state_start_ms = get_tim2_ms();

	while (1)
	{
		now = get_tim2_ms();

		if (night_request)
		{
			night_request = false;
			mode = MODE_NIGHT;
			blink_count = 0;
			last_blink_ms = now;
			set_leds(0, 1, 0);
			tm1637_str(&seg, "88.88");
			night_digit_display = true;
		}

		switch (mode)
		{
		case MODE_DAY:
			day_fsm_run(now);
			break;

		case MODE_NIGHT:
			night_fsm_run(now);
			break;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static uint32_t last_exti_ms = 0;
	uint32_t now = get_tim2_ms();

	if (GPIO_Pin == B1_Pin)
	{
		if (now - last_exti_ms > 200) // 디바운스
		{
			night_request = true;
			last_exti_ms = now;
		}
	}
}
