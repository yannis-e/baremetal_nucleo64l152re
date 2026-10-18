
#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct uart {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
};

#define UART2 ((struct uart *)0x40004400UL)

void uart_init(struct uart *uart, uint32_t pclk, uint32_t baud);
void uart_write_byte(struct uart *uart, uint8_t byte);
void uart_write_buf(struct uart *uart, const char *buf, size_t len);
bool uart_read_ready(struct uart *uart);
uint8_t uart_read_byte(struct uart *uart);

#endif
