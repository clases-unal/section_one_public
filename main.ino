#include "hal_gpio.h"
#include "hal_adc.h"
#include "hal_pwm.h"
#include "control_logic.h"

/* ============================= */
/* DEFINICIÓN DE PINES           */
/* ============================= */

#define BTN_PWM      2
#define BTN_ADC      3
#define BTN_TIMER    4

#define POT_DUTY     A0
#define POT_FREQ     A1

#define PWM_PIN      9
#define FIXED_PWM_FREQ  4000      // Frecuencia fija base (Hz)
#define DEBOUNCE_MS     50


/* ============================= */
/* ESTADOS DEL SISTEMA           */
/* ============================= */

/* Botones tipo toggle */
bool pwm_enabled = false;
bool adc_enabled = false;

/* Modo de frecuencia (controlado por botón TIMER) */
freq_range_mode_t freq_mode = FREQ_FIXED;


/* ============================= */
/* VARIABLES PARA DEBOUNCE       */
/* ============================= */

bool btn_pwm_stable = HIGH;
bool btn_pwm_last   = HIGH;
unsigned long btn_pwm_time = 0;

bool btn_adc_stable = HIGH;
bool btn_adc_last   = HIGH;
unsigned long btn_adc_time = 0;

bool btn_timer_stable = HIGH;
bool btn_timer_last   = HIGH;
unsigned long btn_timer_time = 0;


/* Debug */
unsigned long last_debug_time = 0;


/* ============================= */
/* SETUP                         */
/* ============================= */

void setup()
{
    Serial.begin(115200);

    /* Inicialización GPIO */
    hal_gpio_init(BTN_PWM, HAL_GPIO_INPUT_PULLUP);
    hal_gpio_init(BTN_ADC, HAL_GPIO_INPUT_PULLUP);
    hal_gpio_init(BTN_TIMER, HAL_GPIO_INPUT_PULLUP);

    /* Inicialización ADC */
    hal_adc_init(POT_DUTY);
    hal_adc_init(POT_FREQ);

    /* Inicialización PWM rápido (Timer1) */
    hal_pwm_fast_init();

    Serial.println("Sistema iniciado.");
}


/* ============================= */
/* LOOP PRINCIPAL                */
/* ============================= */

void loop()
{
    /* ========================================= */
    /* ACTUALIZAR BOTONES (con debounce)        */
    /* ========================================= */

    pwm_enabled = button_update(pwm_enabled,
                                hal_gpio_read(BTN_PWM),
                                &btn_pwm_stable,
                                &btn_pwm_last,
                                &btn_pwm_time,
                                DEBOUNCE_MS);

    adc_enabled = button_update(adc_enabled,
                                hal_gpio_read(BTN_ADC),
                                &btn_adc_stable,
                                &btn_adc_last,
                                &btn_adc_time,
                                DEBOUNCE_MS);

    /* Botón TIMER no es toggle booleano:
       genera evento para cambiar de modo */
    bool timer_event = button_update(false,
                                     hal_gpio_read(BTN_TIMER),
                                     &btn_timer_stable,
                                     &btn_timer_last,
                                     &btn_timer_time,
                                     DEBOUNCE_MS);

    /* Si hubo evento, avanzar modo */
    if(timer_event)
    {
        freq_mode = (freq_range_mode_t)((freq_mode + 1) % FREQ_MODES_COUNT);
    }


    /* ========================================= */
    /* LECTURA DE POTENCIÓMETROS                */
    /* ========================================= */

    uint16_t duty_adc = hal_adc_read(POT_DUTY);
    uint16_t freq_adc = hal_adc_read(POT_FREQ);


    /* ========================================= */
    /* CÁLCULO DE SALIDA FINAL                  */
    /* ========================================= */

    /* Calcular Duty final según reglas:
       - PWM apagado → 0%
       - PWM activo pero ADC apagado → 100%
       - Ambos activos → potenciómetro */
    uint8_t duty_percent = compute_final_duty(pwm_enabled,
                                              adc_enabled,
                                              duty_adc);

    /* Calcular frecuencia final:
       - PWM o ADC apagado → fija 4kHz
       - Modo fijo → 4kHz
       - Modo rango → depende del potenciómetro */
    uint32_t frequency = compute_final_frequency(pwm_enabled,
                                                 adc_enabled,
                                                 freq_mode,
                                                 freq_adc,
                                                 FIXED_PWM_FREQ);


    /* ========================================= */
    /* APLICAR PWM                              */
    /* ========================================= */

    hal_pwm_fast_set_frequency(frequency);
    hal_pwm_fast_set_duty_percent(duty_percent);


    /* ========================================= */
    /* DEBUG SERIAL (cada 200ms)                */
    /* ========================================= */

    if(millis() - last_debug_time > 200)
    {
        last_debug_time = millis();

        Serial.print("MODOS: [PWM:");
        Serial.print(pwm_enabled ? "ON " : "OFF");

        Serial.print(" | ADC:");
        Serial.print(adc_enabled ? "ON " : "OFF");

        Serial.print(" | MODE:");
        Serial.print(freq_mode);

        Serial.print("]  -->  ");

        Serial.print("SALIDA: ");
        Serial.print(duty_percent);
        Serial.print("% @ ");
        Serial.print(frequency);
        Serial.println(" Hz");
    }
}
