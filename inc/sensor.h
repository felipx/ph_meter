/*
 * sensor.h
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#ifndef SENSOR_H_
#define SENSOR_H_


#include <stdint.h>

#include "lcd5110.h"


void init_sensor_view(void);
void exit_sensor_view(void);
void ph_view(void);
void mv_view(void);


#endif /* SENSOR_H_ */
