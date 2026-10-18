#include "drivers/systick.h"

#define BIT(n) (1UL << (n))

struct systick {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
};

#define SYSTICK ((struct systick *)0xE000E010UL)

static volatile uint32_t s_ticks;

void SysTick_Handler(void)
{
    s_ticks++;
}

void systick_init(uint32_t ticks_per_ms)
{
    if (ticks_per_ms == 0U || ticks_per_ms > 0x1000000UL) {
        for (;;) {
            __asm volatile ("");
        }
    }

    s_ticks = 0U;

    SYSTICK->CTRL = 0U;
    SYSTICK->LOAD = ticks_per_ms - 1U;
    SYSTICK->VAL = 0U;

    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);
}

uint32_t millis(void)
{
    return s_ticks;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = millis();

    while ((uint32_t)(millis() - start) < ms) {
        __asm volatile ("");
    }
}