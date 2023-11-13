/*
 * sensor.h
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#ifndef SENSOR_H_
#define SENSOR_H_


#include <stdint.h>

#define SENSOR_ADDR (0x20080040)
#define DATA_ADDR (0x2007C000)
#define N_SAMPLES (1024)

typedef struct {

    uint32_t (*read_samples) (void);

} SENSOR_t;


void init_sensor(SENSOR_t *sensor);


#endif /* SENSOR_H_ */
