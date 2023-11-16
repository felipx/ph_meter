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

void init_cal_start(void);
void exit_cal_start(void);
void init_cal(void);
void exit_cal(void);
void one_point_cal_start(void);
void one_point_cal(void);
void two_point_cal_start(void);
void two_point_cal(void);
void three_point_cal_start(void);
void three_point_cal(void);
void calibration_complete(void);


#endif /* CALIBRATION_H_ */
