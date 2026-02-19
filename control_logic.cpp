#include "control_logic.h"

/* Detecta flanco de bajada con debounce */
bool button_update(bool current_state,
                   bool raw_reading,
                   bool *stable_state,
                   bool *last_reading,
                   unsigned long *last_time,
                   unsigned long debounce_ms)
{
    if(raw_reading != *last_reading)
        *last_time = millis();

    if((millis() - *last_time) > debounce_ms)
    {
        if(raw_reading != *stable_state)
        {
            *stable_state = raw_reading;

            if(*stable_state == LOW)
                current_state = !current_state;
        }
    }

    *last_reading = raw_reading;

    return current_state;
}

/* 0-1023 → 0-100% */
uint8_t adc_to_percent(uint16_t adc_value)
{
    return (adc_value * 100UL) / 1023;
}

/* 0-1023 → 0-255 */
uint8_t adc_to_pwm8(uint16_t adc_value)
{
    return adc_value >> 2;
}

uint32_t adc_to_frequency(uint16_t adc_value) 
{
    return 1 + ((adc_value * 9999UL) / 1023);
}


uint32_t compute_final_frequency(bool pwm_enabled,
                                 bool adc_enabled,
                                 freq_range_mode_t mode,
                                 uint16_t adc_freq_value,
                                 uint32_t fixed_frequency)
{
    /* Si PWM no está activo → frecuencia irrelevante */
    if(!pwm_enabled)
        return fixed_frequency;

    /* Si ADC no activo → frecuencia fija */
    if(!adc_enabled)
        return fixed_frequency;

    /* Si modo fijo seleccionado */
    if(mode == FREQ_FIXED)
        return fixed_frequency;

    /* Modos por rango */
    switch(mode)
    {
        case FREQ_1_10:
            return map(adc_freq_value, 0, 1023, 1, 10);

        case FREQ_10_100:
            return map(adc_freq_value, 0, 1023, 10, 100);

        case FREQ_100_1K:
            return map(adc_freq_value, 0, 1023, 100, 1000);

        case FREQ_1K_4K:
            return map(adc_freq_value, 0, 1023, 1000, 4000);

        case FREQ_4K_10K:
            return map(adc_freq_value, 0, 1023, 4000, 10000);

        default:
            return fixed_frequency;
    }
}


/* Lógica de Duty Cycle según tus reglas */
uint8_t compute_final_duty(bool pwm_enabled,
                           bool adc_enabled,
                           uint16_t adc_value)
{
    // Si PWM (pulsador 1) no está activo -> 0%
    if(!pwm_enabled)
        return 0;

    // Si PWM activo pero ADC (pulsador 2) NO pulsado -> 100%
    if(!adc_enabled)
        return 100;

    // Si ambos están activos -> valor del potenciómetro 1 (0-100%)
    return adc_to_percent(adc_value);
}
