#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "adc-task.h"

#define ADC_GPIO_PIN   26
#define ADC_CHANNEL    0
#define TEMP_SENSOR_CHANNEL 4 

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