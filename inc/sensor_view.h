/*
 * sensor_view.h
 *
 *  Created on: Nov 13, 2023
 *      Author: felipe
 */

#ifndef SENSOR_VIEW_H_
#define SENSOR_VIEW_H_


#include "lcd5110.h"


void init_sensor_view(void);
void exit_sensor_view(void);
void ph_view(void);
void mv_view(void);


#endif /* SENSOR_VIEW_H_ */
