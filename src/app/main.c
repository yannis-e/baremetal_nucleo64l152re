#include "drivers/gpio.h"
#include "drivers/systick.h"
#include "drivers/pwm.h"

#include <stdint.h>
#include <stdbool.h>

#define LED_COUNT 4U
#define PWM_PERIOD 100U
#define FADE_INTERVAL_MS 3U

static const uint32_t led_pins[LED_COUNT] = {
    PIN('A', 5),
    PIN('A', 6),
    PIN('A', 7),
    PIN('B', 6)
};

static const uint8_t phase_offsets[LED_COUNT] = {
    0U, 50U, 100U, 150U
};

static const uint8_t gamma_table[101] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 2, 2, 2, 2, 3,
    3, 3, 4, 4, 4, 5, 5, 6, 6, 7,
    7, 8, 8, 9, 9, 10, 11, 11, 12, 13,
    13, 14, 15, 16, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    33, 34, 35, 36, 37, 39, 40, 41, 43, 44,
    46, 47, 49, 50, 52, 53, 55, 56, 58, 60,
    61, 63, 65, 66, 68, 70, 72, 74, 75, 77,
    79, 81, 83, 85, 87, 89, 91, 94, 96, 98,
    100
};

static uint16_t brightness_to_duty(uint8_t brightness)
{
    if (brightness > 100U) {
        brightness = 100U;
    }

    return gamma_table[brightness];
}

static uint8_t triangle_brightness(uint8_t phase)
{
    if (phase <= 100U) {
        return phase;
    }

    return (uint8_t)(200U - phase);
}

int main(void)
{
    systick_init(2097U);

    Pwm led_pwm[LED_COUNT];

    for (uint8_t i = 0U; i < LED_COUNT; i++) {
        gpio_set_mode(led_pins[i], GPIO_MODE_OUTPUT);
        gpio_write(led_pins[i], true);

        pwmInit(&led_pwm[i], PWM_PERIOD);
        pwmSetDuty(&led_pwm[i], 0U);
    }

    uint8_t phase = 0U;
    uint32_t last_fade = millis();

    for (;;) {
        for (uint8_t i = 0U; i < LED_COUNT; i++) {
            uint8_t led_phase =
                (uint8_t)((phase + phase_offsets[i]) % 200U);

            uint8_t brightness = triangle_brightness(led_phase);

            pwmSetDuty(
                &led_pwm[i],
                brightness_to_duty(brightness)
            );

            gpio_write(led_pins[i], !pwmUpdate(&led_pwm[i]));
        }

        uint32_t now = millis();

        if ((uint32_t)(now - last_fade) >= FADE_INTERVAL_MS) {
            last_fade = now;
            phase++;

            if (phase >= 200U) {
                phase = 0U;
            }
        }
    }
}