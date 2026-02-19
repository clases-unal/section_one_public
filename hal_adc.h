#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <Arduino.h>

/* Inicializa pin analógico */
void hal_adc_init(uint8_t pin);

/* Lee valor ADC (0-1023) */
uint16_t hal_adc_read(uint8_t pin);

#endif
