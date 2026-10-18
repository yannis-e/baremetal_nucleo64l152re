
#include "drivers/uart.h"
#include "drivers/gpio.h"

#define BIT(n) (1UL << (n))

#define RCC_BASE 0x40023800UL

struct rcc {
    uint32_t reserved0[7];
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
};

#define RCC ((struct rcc *)RCC_BASE)

void uart_init(struct uart *uart, uint32_t pclk, uint32_t baud)
{
    if (uart == UART2) {
        RCC->AHBENR |= BIT(0);    
        RCC->APB1ENR |= BIT(17);  

        gpio_set_mode(PIN('A', 2), GPIO_MODE_AF);
        gpio_set_af(PIN('A', 2), 7U);

        gpio_set_mode(PIN('A', 3), GPIO_MODE_AF);
        gpio_set_af(PIN('A', 3), 7U);
    }

    if (baud == 0U || pclk == 0U) {
        for (;;) {
        }
    }

    uart->CR1 = 0U;
    uart->CR2 = 0U;
    uart->CR3 = 0U;

    uart->BRR = (pclk + baud / 2U) / baud;
    uart->CR1 = BIT(13) | BIT(3) | BIT(2); 
}

void uart_write_byte(struct uart *uart, uint8_t byte)
{
    while ((uart->SR & BIT(7)) == 0U) {
    }

    uart->DR = byte;
}

void uart_write_buf(struct uart *uart, const char *buf, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            uart_write_byte(uart, '\r');
        }

        uart_write_byte(uart, (uint8_t)buf[i]);
    }
}

bool uart_read_ready(struct uart *uart)
{
    return (uart->SR & BIT(5)) != 0U;
}

uint8_t uart_read_byte(struct uart *uart)
{
    return (uint8_t)(uart->DR & 0xFFU);
}
