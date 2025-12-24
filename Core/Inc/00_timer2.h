#pragma once

#include <stdint.h>

typedef void (*timer_cb_t)(void);

void timer2_run(void);
void tim2_register_callback(timer_cb_t cb);
void tim2_unregister_all(void);
uint32_t get_tim2_ms(void);
