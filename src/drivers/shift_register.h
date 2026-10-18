
#ifndef DRIVERS_SHIFT_REGISTER_H
#define DRIVERS_SHIFT_REGISTER_H

#include <stdint.h>

void shift_register_init(uint16_t sdi,
                         uint16_t sftclk,
                         uint16_t lchclk);

void shift_register_write(uint8_t data1,
                          uint8_t data2);

#endif
