#include "hal_gpio.h"

void hal_gpio_init(uint8_t pin, hal_gpio_mode_t mode)
{
    if(mode == HAL_GPIO_INPUT)
        pinMode(pin, INPUT);
    else if(mode == HAL_GPIO_OUTPUT)
        pinMode(pin, OUTPUT);
    else
        pinMode(pin, INPUT_PULLUP);
}

void hal_gpio_write(uint8_t pin, bool state)
{
    digitalWrite(pin, state ? HIGH : LOW);
}

bool hal_gpio_read(uint8_t pin)
{
    return digitalRead(pin);
}
