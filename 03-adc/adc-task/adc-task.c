#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "adc-task.h"

#define ADC_GPIO_PIN   26
#define ADC_CHANNEL    0
#define TEMP_SENSOR_CHANNEL 4 

static adc_task_state_t adc_state = ADC_TASK_STATE_IDLE;
static uint64_t last_measure_time_us;

static const uint64_t ADC_TASK_MEAS_PERIOD_US = 100000;

void adc_task_init(void)
{
    adc_init();
    adc_gpio_init(ADC_GPIO_PIN);
    adc_set_temp_sensor_enabled(true);
}

float adc_task_measure_voltage(void)
{
    adc_select_input(ADC_CHANNEL);
    uint16_t voltage_counts = adc_read();
    float voltage_V = (voltage_counts / 4095.0f) * 3.3f;
    return voltage_V;
}

float adc_task_measure_temperature(void)
{   adc_select_input(TEMP_SENSOR_CHANNEL);
    
    uint16_t temp_counts = adc_read();
    
    float temp_V = (temp_counts / 4095.0f) * 3.3f;
    
    float temp_C = 27.0f - (temp_V - 0.706f) / 0.001721f;
    
    return temp_C;
}

void adc_task_set_state(adc_task_state_t state)
{
    adc_state = state;
}

void adc_task_handle(void)
{
    if (adc_state == ADC_TASK_STATE_RUN) {
        uint64_t current_time_us = time_us_64();
        
        if ((current_time_us - last_measure_time_us) >= ADC_TASK_MEAS_PERIOD_US) {
            float voltage_V = adc_task_measure_voltage();
            float temp_C = adc_task_measure_temperature();
            printf("%f %f\n", voltage_V, temp_C);
            last_measure_time_us = current_time_us;
        }
    }
}