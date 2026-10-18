#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_init(uint32_t ticks_per_ms);

uint32_t millis(void);

void delay_ms(uint32_t ms);

#endif