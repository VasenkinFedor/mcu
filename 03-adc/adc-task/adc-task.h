#ifndef ADC_TASK_H
#define ADC_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

void adc_task_init(void);
float adc_task_measure_voltage(void);
float adc_task_measure_temperature(void);

#ifdef __cplusplus
}
#endif

#endif // ADC_TASK_H