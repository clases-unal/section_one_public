#include "hal_pwm.h"

/* ============================= */
/* PWM NORMAL (analogWrite)      */
/* Frecuencia fija (~490Hz)      */
/* ============================= */

void hal_pwm_normal_init(uint8_t pin)
{
    pinMode(pin, OUTPUT);
}

void hal_pwm_normal_set(uint8_t pin, uint8_t duty)
{
    analogWrite(pin, duty);   // duty 0-255
}

/* ============================= */
/* PWM RÁPIDO (Timer1 hardware) */
/* Frecuencia variable           */
/* ============================= */

void hal_pwm_fast_init(void)
{
    pinMode(9, OUTPUT);

    TCCR1A = 0;
    TCCR1B = 0;

    /* Modo Fast PWM con ICR1 como TOP */
    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);

    hal_pwm_fast_set_frequency(1000);
    hal_pwm_fast_set_duty_percent(50);
}

void hal_pwm_fast_set_frequency(uint32_t freq_hz)
{
    uint32_t top;
    uint16_t prescaler_bits = 0;
    uint32_t prescaler = 1;

    /* Intentar prescalers de menor a mayor para mejor resolución */
    
    if((16000000UL / freq_hz) <= 65535UL)
    {
        prescaler = 1;
        prescaler_bits = (1 << CS10);
    }
    else if((16000000UL / 8 / freq_hz) <= 65535UL)
    {
        prescaler = 8;
        prescaler_bits = (1 << CS11);
    }
    else if((16000000UL / 64 / freq_hz) <= 65535UL)
    {
        prescaler = 64;
        prescaler_bits = (1 << CS11) | (1 << CS10);
    }
    else if((16000000UL / 256 / freq_hz) <= 65535UL)
    {
        prescaler = 256;
        prescaler_bits = (1 << CS12);
    }
    else
    {
        prescaler = 1024;
        prescaler_bits = (1 << CS12) | (1 << CS10);
    }

    top = (16000000UL / (prescaler * freq_hz)) - 1;

    if(top > 65535)
        top = 65535;

    /* Resetear prescaler */
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));

    /* Aplicar nuevo prescaler */
    TCCR1B |= prescaler_bits;

    ICR1 = top;
}


void hal_pwm_fast_set_duty_percent(uint8_t duty_percent) {
    if(duty_percent >= 100) {
        OCR1A = ICR1 + 1; // Fuerza salida constante en alto
    } else if(duty_percent == 0) {
        OCR1A = 0;
    } else {
        OCR1A = (uint32_t)ICR1 * duty_percent / 100;
    }
}
