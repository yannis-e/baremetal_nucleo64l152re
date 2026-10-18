#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>
#include <stdint.h>

enum gpio_mode {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
};

/* Encode a port and pin, e.g. PIN('A', 5). */
#define PIN(bank, num) \
    ((((bank) - 'A') << 8) | (num))

void gpio_set_mode(uint16_t pin, enum gpio_mode mode);
void gpio_write(uint16_t pin, bool value);
bool gpio_read(uint16_t pin);
void gpio_set_af(uint16_t pin, uint8_t af);

#define GPIO_PULL_NONE 0U
#define GPIO_PULL_UP   1U
#define GPIO_PULL_DOWN 2U

void gpio_set_pull(uint16_t pin, uint32_t pull);

#endif