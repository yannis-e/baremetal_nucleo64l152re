#include "drivers/shift_register.h"
#include "drivers/gpio.h"

static uint16_t sdi_pin;
static uint16_t sftclk_pin;
static uint16_t lchclk_pin;

static void shift_out_byte(uint8_t data)
{
    for (int i = 7; i >= 0; i--) {
        bool bit = ((data >> i) & 1U) != 0U;

        gpio_write(sdi_pin, bit);

        gpio_write(sftclk_pin, true);
        gpio_write(sftclk_pin, false);
    }
}

void shift_register_init(uint16_t sdi,
                         uint16_t sftclk,
                         uint16_t lchclk)
{
    sdi_pin = sdi;
    sftclk_pin = sftclk;
    lchclk_pin = lchclk;

    gpio_set_mode(sdi_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(sftclk_pin, GPIO_MODE_OUTPUT);
    gpio_set_mode(lchclk_pin, GPIO_MODE_OUTPUT);

    gpio_write(sdi_pin, false);
    gpio_write(sftclk_pin, false);
    gpio_write(lchclk_pin, false);
}

void shift_register_write(uint8_t data1, uint8_t data2)
{
    gpio_write(lchclk_pin, false);
    gpio_write(sftclk_pin, false);

    shift_out_byte(data1);
    shift_out_byte(data2);

    gpio_write(lchclk_pin, true);
    gpio_write(lchclk_pin, false);
}
