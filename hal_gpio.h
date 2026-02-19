#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <Arduino.h>

/* Modos básicos */
typedef enum {
    HAL_GPIO_INPUT,
    HAL_GPIO_OUTPUT,
    HAL_GPIO_INPUT_PULLUP
} hal_gpio_mode_t;

/* Configura el modo del pin */
void hal_gpio_init(uint8_t pin, hal_gpio_mode_t mode);

/* Escribe nivel lógico */
void hal_gpio_write(uint8_t pin, bool state);

/* Lee nivel lógico */
bool hal_gpio_read(uint8_t pin);

#endif
