#include "drivers/gpio.h"

#define BIT(n) (1UL << (n))

#define GPIO_BASE 0x40020000UL
#define GPIO_STRIDE 0x400UL
#define RCC_BASE 0x40023800UL

struct gpio {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
    volatile uint32_t BRR;
};

struct rcc {
    uint32_t reserved[7];
    volatile uint32_t AHBENR;
};

#define RCC ((struct rcc *)RCC_BASE)

static struct gpio *gpio_port(uint16_t pin)
{
    uint32_t bank = (uint32_t)pin >> 8;

    return (struct gpio *)(GPIO_BASE + GPIO_STRIDE * bank);
}

static void gpio_enable_clock(uint16_t pin)
{
    uint32_t bank = (uint32_t)pin >> 8;

    RCC->AHBENR |= BIT(bank);
    (void)RCC->AHBENR;
}

void gpio_set_mode(uint16_t pin, enum gpio_mode mode)
{
    struct gpio *gpio = gpio_port(pin);
    uint32_t n = (uint32_t)pin & 0xFFU;

    gpio_enable_clock(pin);

    gpio->MODER &= ~(3UL << (n * 2U));
    gpio->MODER |= ((uint32_t)mode & 3U) << (n * 2U);
}

void gpio_write(uint16_t pin, bool value)
{
    struct gpio *gpio = gpio_port(pin);
    uint32_t n = (uint32_t)pin & 0xFFU;

    gpio->BSRR = BIT(n) << (value ? 0U : 16U);
}

bool gpio_read(uint16_t pin) {
    struct gpio *gpio = gpio_port(pin);
    uint32_t n = (uint32_t)pin & 0xFFU;

    return (gpio->IDR & BIT(n)) != 0U;
}

void gpio_set_af(uint16_t pin, uint8_t af) {
    struct gpio *gpio = gpio_port(pin);
    uint32_t n = (uint32_t)pin & 0xFFU;
    volatile uint32_t *afr = (n < 8U)
        ? &gpio->AFRL
        : &gpio->AFRH;

    uint32_t shift = (n % 8U) * 4U;

    gpio_enable_clock(pin);

    *afr &= ~(0xFUL << shift);
    *afr |= ((uint32_t)af & 0xFU) << shift;
}


void gpio_set_pull(uint16_t pin, uint32_t pull)
{
    struct gpio *gpio = gpio_port(pin);
    uint32_t n = (uint32_t)pin & 0xFFU;
    uint32_t shift = n * 2U;

    gpio_enable_clock(pin);

    gpio->PUPDR &= ~(3UL << shift);
    gpio->PUPDR |= (pull & 3U) << shift;
}
