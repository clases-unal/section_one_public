#ifndef HAL_PWM_H
#define HAL_PWM_H

#include <Arduino.h>

/* PWM simple usando analogWrite */
void hal_pwm_normal_init(uint8_t pin);
void hal_pwm_normal_set(uint8_t pin, uint8_t duty);

/* PWM avanzado usando Timer1 (pin 9) */
void hal_pwm_fast_init(void);
void hal_pwm_fast_set_frequency(uint32_t freq_hz);
void hal_pwm_fast_set_duty_percent(uint8_t duty_percent);

#endif
