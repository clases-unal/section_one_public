#include "hal_adc.h"

void hal_adc_init(uint8_t pin)
{
    pinMode(pin, INPUT);
}

uint16_t hal_adc_read(uint8_t pin)
{
    return analogRead(pin);
}
