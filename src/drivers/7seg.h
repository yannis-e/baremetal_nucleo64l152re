
#ifndef DRIVERS_7seg_H
#define DRIVERS_7seg_H

#include <stdint.h>
#include <stdbool.h>

void displayInit(bool leadingZeros);

void displaySet(uint16_t number);

void displaySetDot(uint8_t digit, bool enabled);

void displayReload();

#endif
