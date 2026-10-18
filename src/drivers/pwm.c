#include "pwm.h"

void pwmInit(Pwm *pwm, uint16_t period)
{
    pwm->counter = 0U;
    pwm->period = period;
    pwm->duty = 0U;
}

void pwmSetDuty(Pwm *pwm, uint16_t duty) {
    if(duty > pwm->period) {
        duty = pwm->period;
    }
    pwm->duty = duty;
}

bool pwmUpdate(Pwm *pwm) {
    bool output = pwm->counter < pwm->duty;

    pwm->counter++;

    if(pwm->counter >= pwm->period) {
        pwm->counter = 0U;
    }

    return output;
}