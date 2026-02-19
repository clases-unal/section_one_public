#ifndef CONTROL_LOGIC_H
#define CONTROL_LOGIC_H

#include <Arduino.h>

/* Maneja debounce y toggle */
bool button_update(bool current_state,
                   bool raw_reading,
                   bool *stable_state,
                   bool *last_reading,
                   unsigned long *last_time,
                   unsigned long debounce_ms);

/* Conversión ADC */
uint8_t adc_to_percent(uint16_t adc_value);
uint8_t adc_to_pwm8(uint16_t adc_value);
uint32_t adc_to_frequency(uint16_t adc_value);

/* Lógica de duty final */
uint8_t compute_final_duty(bool pwm_enabled,
                           bool adc_enabled,
                           uint16_t adc_value);

typedef enum {
    FREQ_FIXED = 0,   // 4kHz fija
    FREQ_1_10,        // 1 - 10 Hz
    FREQ_10_100,      // 10 - 100 Hz
    FREQ_100_1K,      // 0.1 - 1 kHz
    FREQ_1K_4K,       // 1 - 4 kHz
    FREQ_4K_10K,      // 4 - 10 kHz
    FREQ_MODES_COUNT  // Auxiliar para el ciclo
} freq_range_mode_t;

uint32_t compute_final_frequency(bool pwm_enabled,
                                 bool adc_enabled,
                                 freq_range_mode_t mode,
                                 uint16_t adc_freq_value,
                                 uint32_t fixed_frequency);
#endif
