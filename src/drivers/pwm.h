#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t counter;
    uint16_t period;
    uint16_t duty;
} Pwm;

void pwmInit(Pwm *pwm, uint16_t period);
void pwmSetDuty(Pwm *pwm, uint16_t duty);
bool pwmUpdate(Pwm *pwm);

#endif