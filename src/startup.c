
#include <stdint.h>

extern int main(void);

extern uint32_t _estack;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;

void SysTick_Handler(void);

void Default_Handler(void)
{
    for (;;) {
        __asm volatile ("");
    }
}

__attribute__((noreturn))
void _reset(void)
{
    /* Clear the .bss section. */
    for (uint32_t *dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0;
    }

    /* Copy initialized data from Flash into RAM. */
    for (uint32_t *dst = &_sdata, *src = &_sidata;
         dst < &_edata;
         dst++, src++) {
        *dst = *src;
    }

    (void)main();

    for (;;) {
        __asm volatile ("");
    }
}

/* Cortex-M3 vector table: 16 core entries + 43 external IRQs. */
__attribute__((section(".vectors"), used))
void (*const vectors[16 + 43])(void) = {
    [0]  = (void (*)(void))(&_estack),
    [1]  = _reset,
    [2]  = Default_Handler,  /* NMI */
    [3]  = Default_Handler,  /* HardFault */
    [4]  = Default_Handler,  /* MemManage */
    [5]  = Default_Handler,  /* BusFault */
    [6]  = Default_Handler,  /* UsageFault */
    [11] = Default_Handler,  /* SVCall */
    [12] = Default_Handler,  /* Debug monitor */
    [14] = Default_Handler,  /* PendSV */
    [15] = SysTick_Handler
};
