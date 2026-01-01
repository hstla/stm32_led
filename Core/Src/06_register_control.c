#include <stdint.h>
#include "06_register_control.h"

// 베이스 주소
#define GPIOA_BASE 0x40020000UL
#define GPIOC_BASE 0x40020800UL
#define RCC_BASE 0x40023800UL

// RCC 레지스터
#define RCC_AHB1ENR  (*(volatile uint32_t *)(RCC_BASE  + 0x30))

// GPIOA 레지스터
#define GPIOA_MODER (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_PUPDR (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_BSRR (*(volatile uint32_t *)(GPIOA_BASE + 0x18))

// GPIOC 레지스터
#define GPIOC_MODER (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_PUPDR (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))
#define GPIOC_IDR (*(volatile uint32_t *)(GPIOC_BASE + 0x10))

#define LD2_PIN 5
#define BTN_PIN 13


void gpio_register_run(void)
{
	// RCC에서 GPIOA, GPIOC 클럭 Enable 설정
	RCC_AHB1ENR |= (1U << 0);
	RCC_AHB1ENR |= (1U << 2);

	// 내부 LED 설정
	GPIOA_MODER &= ~(0x3U << (LD2_PIN * 2));
	GPIOA_MODER |= (0x1U << (LD2_PIN * 2));

	// PC13 내부 버튼 설정
	GPIOC_MODER &= ~(0x3U << (BTN_PIN * 2));
	GPIOC_PUPDR &= ~(0x3U << (BTN_PIN * 2));

	while(1)
	{
		uint32_t btn = (GPIOC_IDR >> BTN_PIN) & 0x1;

		if (btn == 0) {
			GPIOA_BSRR = (1U << LD2_PIN);
		}
		else {
			GPIOA_BSRR = (1U << (LD2_PIN + 16));
		}
	}
}
