/*
 * sensor.c
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#include "sensor.h"


static uint32_t read_samples(void)
{
    uint32_t *adc_data = (uint32_t *) DATA_ADDR;
    uint32_t n_samples = (uint32_t) N_SAMPLES;
    uint32_t adc_val = 0;

    for (uint32_t i=0; i<n_samples; i++)
        adc_val += (*(adc_data + i) >> 4) & 0xFFF;
    adc_val /= n_samples;

    return adc_val;
}


void init_sensor(SENSOR_t *sensor)
{
    sensor->read_samples = read_samples;
}
