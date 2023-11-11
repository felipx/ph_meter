/*
 * calibration.h
 *
 *  Created on: Nov 9, 2023
 *      Author: felipe
 */

#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include "lcd5110.h"

extern float offset;
extern float slope;

extern uint32_t *adc_ph_data;
extern uint32_t adc_ph_samples;

void init_cal_start(void);
void exit_cal_start(void);
void init_cal(void);
void exit_cal(void);
void one_point_cal_start(LCD5110_t *lcd5110);
void one_point_cal(LCD5110_t *lcd5110);
void two_point_cal_start(LCD5110_t *lcd5110);
void two_point_cal(LCD5110_t *lcd5110);
void calibration_complete(LCD5110_t *lcd5110);


#endif /* CALIBRATION_H_ */
