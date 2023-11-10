/*
 * calibration.c
 *
 *  Created on: Nov 9, 2023
 *      Author: felipe
 */

#include <stdio.h>
#include <lpc17xx_timer.h>

#include "fsm.h"
#include "calibration.h"

#define COUNT_START 30

float offset = 3.3;
float slope = -0.2357;

uint32_t *adc_ph_data = (uint32_t *) 0x2007C000;
uint32_t adc_ph_samples = 1024;


static TIM_MATCHCFG_Type timer1_match0_cfg = {.MatchChannel = 0,
		                                      .IntOnMatch = ENABLE,
											  .StopOnMatch = DISABLE,
											  .ResetOnMatch = ENABLE,
                                              .ExtMatchOutputType = TIM_EXTMATCH_NOTHING,
											  .MatchValue = 9999};

static uint8_t count = COUNT_START;
static uint32_t adc_val = 0;
static uint32_t cal_buffer[COUNT_START];

void init_cal_start(void)
{
	// maybe match1.0 menu config here
    TIM_ResetCounter(LPC_TIM1);
    TIM_Cmd(LPC_TIM1, ENABLE);
}


void exit_cal_start(void)
{
    TIM_Cmd(LPC_TIM1, DISABLE);
}


void init_cal(void)
{
	TIM_ConfigMatch(LPC_TIM1, &timer1_match0_cfg);
	TIM_ResetCounter(LPC_TIM1);
	TIM_Cmd(LPC_TIM1, ENABLE);
}


void exit_cal(void)
{
	count = COUNT_START;
    TIM_Cmd(LPC_TIM1, DISABLE);
}


void one_point_cal_start(LCD5110_t *lcd5110)
{
    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("1 POINT CAL");
    lcd5110->set_cursor(1,0);
    lcd5110->print_str("Insert Probe");
    lcd5110->set_cursor(2,0);
    lcd5110->print_str("Into pH 7");
    lcd5110->set_cursor(3,0);
    lcd5110->print_str("Solution. Then");
    lcd5110->set_cursor(4,0);
    lcd5110->print_str("Press ENTER to");
    lcd5110->set_cursor(5,0);
    lcd5110->print_str("Start");
}


void one_point_cal(LCD5110_t *lcd5110)
{
	char count_buf[8];

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("1 POINT CAL");
    lcd5110->set_cursor(1,5);
    lcd5110->print_str("pH 7");

	snprintf(count_buf, 8, "0:%d", count--);
	//lcd5110->clear();
	lcd5110->set_cursor(3,5);
	lcd5110->print_str(count_buf);

	adc_val = 0;
	for (uint32_t i=0; i<adc_ph_samples; i++)
		adc_val += (*(adc_ph_data + i) >> 4) & 0xFFF;
	adc_val /= adc_ph_samples;

    if (count != 0xFF)
        cal_buffer[count] = adc_val;

    if (count == 0xFF) {
		count = 30;

	    uint32_t cal_avg = 0;
	    uint32_t pH = 7;

        for (uint8_t i=0; i<(uint8_t)COUNT_START; i++)
        	cal_avg += cal_buffer[i];
        cal_avg /= (uint8_t) COUNT_START;

        offset = 3.3;
        slope = (((float)cal_avg/4095)*3.3 - offset)/((float)pH);

        event = EV_CAL_COMPLETE;
	}
}


void calibration_complete(LCD5110_t *lcd5110)
{
    static uint8_t i = 0;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();
    lcd5110->set_cursor(1,1);
    lcd5110->print_str("Calibration");
    lcd5110->set_cursor(3,2);
    lcd5110->print_str("Complete!");

    i++;
    if (i == 3) {
    	i = 0;
    event = EV_CAL_COMPLETE;
    }
}








