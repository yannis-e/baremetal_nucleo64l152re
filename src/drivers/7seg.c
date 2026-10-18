
#include "7seg.h"
#include "gpio.h"
#include "shift_register.h"

#define SDI_PIN     PIN('A', 9)
#define SFTCLK_PIN  PIN('A', 8)
#define LCHCLK_PIN  PIN('B', 5)

#define DISPLAY_DIGITS 4U
#define SEGMENT_BLANK  0xFFU

static const uint8_t digit_segments[10] = {
    0xC0U, /* 0 */
    0xF9U, /* 1 */
    0xA4U, /* 2 */
    0xB0U, /* 3 */
    0x99U, /* 4 */
    0x92U, /* 5 */
    0x82U, /* 6 */
    0xF8U, /* 7 */
    0x80U, /* 8 */
    0x90U  /* 9 */
};

static const uint8_t digit_select[DISPLAY_DIGITS] = {
    0xF1U,
    0xF2U,
    0xF4U,
    0xF8U
};

static uint8_t display_value[DISPLAY_DIGITS] = {
    SEGMENT_BLANK,
    SEGMENT_BLANK,
    SEGMENT_BLANK,
    SEGMENT_BLANK
};

static uint8_t position = 0U;
static bool showLeadingZeros = false;

void displayInit(bool leadingZeros)
{
    shift_register_init(SDI_PIN, SFTCLK_PIN, LCHCLK_PIN);

    showLeadingZeros = leadingZeros;
    position = 0U;
    displaySet(0U);
}

void displaySet(uint16_t number)
{
    if (number > 9999U) {
        number = 9999U;
    }

    uint8_t digits[4];

    for(uint8_t i = 0U; i < DISPLAY_DIGITS; i++ ) {
        uint8_t index = DISPLAY_DIGITS - 1U - i;
        uint8_t digit = (uint8_t)(number % 10U);
        digits[index] = digit;
        number /= 10U;
    }

    bool leading = true;

    for (uint8_t i = 0U; i < DISPLAY_DIGITS; i++) {

        uint8_t digit = digits[i];

        if (digit != 0U) {
            leading = false;
        }

        uint8_t dot = display_value[i] & 0x80U;

        if (leading && !showLeadingZeros &&
            i < DISPLAY_DIGITS - 1U) {
            display_value[i] = SEGMENT_BLANK | dot;
        } else {
            display_value[i] = (digit_segments[digit] & (uint8_t)~0x80U) | dot;
        }
    }
}

void displaySetDot(uint8_t digit, bool enabled)
{
    if (digit >= DISPLAY_DIGITS) {
        return;
    }

    if (enabled) {
        display_value[digit] &= ~(1U << 7);
    } else {
        display_value[digit] |= (1U << 7);
    }
}

void displayReload(void)
{
    shift_register_write(
        display_value[position],
        digit_select[position]
    );

    position++;

    if (position >= DISPLAY_DIGITS) {
        position = 0U;
    }
}
