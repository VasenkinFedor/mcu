#include <stdio.h>
#include <stddef.h>         
#include "pico/stdlib.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "adc-task/adc-task.h" 

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void get_adc_callback(const char* args)
{
    float voltage_V = adc_task_measure_voltage(); 
    printf("%f\n", voltage_V);                     
}

void get_temp_callback(const char* args)
{
    float temp_C = adc_task_measure_temperature();
    printf("%f\n", temp_C);
}

void tm_start_callback(const char* args)
{
    adc_task_set_state(ADC_TASK_STATE_RUN);
    printf("Measurements started\n");
}

void tm_stop_callback(const char* args)
{
    adc_task_set_state(ADC_TASK_STATE_IDLE);
    printf("Measurements stopped\n");
}

api_t device_api[] =
{
    {"version", version_callback, "get device name and firmware version"},
    {"get_adc", get_adc_callback, "measure voltage on GPIO26"},
    {"get_temp", get_temp_callback, "measure internal temperature"},
    {"tm_start", tm_start_callback, "start periodic measurements (voltage and temperature)"},
    {"tm_stop", tm_stop_callback, "stop periodic measurements"},
    {NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();

    stdio_task_init();
    
    protocol_task_init(device_api);

    adc_task_init(); 
    
    while (1)
    {
        char* command = stdio_task_handle();
        if (command != NULL)  
        {
            protocol_task_handle(command);
        }
        
        adc_task_handle();
    }
}