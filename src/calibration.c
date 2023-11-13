/*
 * calibration.c
 *
 *  Created on: Nov 9, 2023
 *      Author: felipe
 */

#include <stdio.h>
#include <lpc17xx_gpdma.h>
#include <lpc17xx_timer.h>

#include "calibration.h"
#include "fsm.h"
#include "sensor.h"

#define COUNT_START 30

float offset = 3.3;
float slope = -0.2357;


// ADC timer 0
static TIM_MATCHCFG_Type timer0_match1_cfg = {.MatchChannel = 1,
                                              .IntOnMatch = ENABLE,
                                              .StopOnMatch = DISABLE,
                                              .ResetOnMatch = ENABLE,
                                              .ExtMatchOutputType = TIM_EXTMATCH_TOGGLE,
                                              .MatchValue = 49};

// lcd5110 refresh timer 1
static TIM_MATCHCFG_Type timer1_match0_cfg = {.MatchChannel = 0,
                                              .IntOnMatch = ENABLE,
                              	              .StopOnMatch = DISABLE,
                                              .ResetOnMatch = ENABLE,
                                              .ExtMatchOutputType = TIM_EXTMATCH_NOTHING,
                                              .MatchValue = 9999};

static uint8_t count = COUNT_START;
static uint32_t cal_buffer[COUNT_START];

static uint8_t point = 0;
static float pH[3] = {7.0, 10.0, 4.0};
static float mV[3] = {0.0, 0.0, 0.0};


void init_cal_start(void)
{
    TIM_ResetCounter(LPC_TIM1);
    TIM_Cmd(LPC_TIM1, ENABLE);
}


void exit_cal_start(void)
{
    TIM_Cmd(LPC_TIM1, DISABLE);
}


void init_cal(void)
{
    TIM_ConfigMatch(LPC_TIM0, &timer0_match1_cfg);
    TIM_ResetCounter(LPC_TIM0);
    TIM_Cmd(LPC_TIM0, ENABLE);

    TIM_ConfigMatch(LPC_TIM1, &timer1_match0_cfg);
    TIM_ResetCounter(LPC_TIM1);
    TIM_Cmd(LPC_TIM1, ENABLE);

    GPDMA_ChannelCmd(0, ENABLE);
}


void exit_cal(void)
{
    count = COUNT_START;
    TIM_Cmd(LPC_TIM0, DISABLE);
    TIM_Cmd(LPC_TIM1, DISABLE);
    GPDMA_ChannelCmd(0, DISABLE);
}


void one_point_cal_start(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;

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


void one_point_cal(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;
    SENSOR_t *ph_sensor = (SENSOR_t *) SENSOR_ADDR;
    char count_buf[8];
    uint32_t adc_val = 0;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    snprintf(count_buf, 8, "0:%d", count--);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("1 POINT CAL");
    lcd5110->set_cursor(1,5);
    lcd5110->print_str("pH 7");
    lcd5110->set_cursor(3,5);
    lcd5110->print_str(count_buf);

    adc_val = ph_sensor->read_samples();

    if (count != 0xFF)
        cal_buffer[count] = adc_val;

    if (count == 0xFF) {
        count = 30;

        uint32_t cal_avg = 0;

        for (uint8_t i=0; i<(uint8_t)COUNT_START; i++)
            cal_avg += cal_buffer[i];
        cal_avg /= (uint8_t) COUNT_START;

        pH[0] = 7.0;
        mV[0] = ((float)cal_avg/4095.0)*3.3;
        offset = 3.3;
        slope = (mV[0]-offset)/((float)pH[0]);

        event = EV_CAL_COMPLETE;
    }
}


void two_point_cal_start(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("2 POINT CAL");
    lcd5110->set_cursor(1,0);
    lcd5110->print_str("Insert Probe");
    lcd5110->set_cursor(2,0);
    if (point == 0)
        lcd5110->print_str("Into pH 4");
    else
        lcd5110->print_str("Into pH 10");
    lcd5110->set_cursor(3,0);
    lcd5110->print_str("Solution. Then");
    lcd5110->set_cursor(4,0);
    lcd5110->print_str("Press ENTER to");
    lcd5110->set_cursor(5,0);
    lcd5110->print_str("Start");
}


void two_point_cal(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;
    SENSOR_t *ph_sensor = (SENSOR_t *) SENSOR_ADDR;
    char count_buf[8];
    uint32_t adc_val = 0;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    snprintf(count_buf, 8, "0:%d", count--);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("2 POINT CAL");
    lcd5110->set_cursor(1,5);
    if (point == 0)
        lcd5110->print_str("pH 4");
    else
        lcd5110->print_str("pH 10");
    lcd5110->set_cursor(3,5);
    lcd5110->print_str(count_buf);

    adc_val = ph_sensor->read_samples();

    if (count != 0xFF)
        cal_buffer[count] = adc_val;

    if (count == 0xFF) {
        count = 30;

        uint32_t cal_avg = 0;

        for (uint8_t i=0; i<(uint8_t)COUNT_START; i++)
            cal_avg += cal_buffer[i];
        cal_avg /= (uint8_t) COUNT_START;

        if (point == 0) {
            point++;
            pH[0] = 4.0;
            mV[0] = ((float)cal_avg/4095.0)*3.3;
            event = EV_POINT_COMPLETE;
        }
        else {
            point = 0;
            pH[1] = 10.0;
            mV[1] = ((float)cal_avg/4095.0)*3.3;
            slope = (mV[1]-mV[0])/(pH[1]-pH[0]);
            offset = mV[1] - slope*pH[1];
            event = EV_CAL_COMPLETE;
        }
    }
}


void three_point_cal_start(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("3 POINT CAL");
    lcd5110->set_cursor(1,0);
    lcd5110->print_str("Insert Probe");
    lcd5110->set_cursor(2,0);
    if (point == 0)
        lcd5110->print_str("Into pH 7");
    else if (point == 1)
        lcd5110->print_str("Into pH 10");
    else
        lcd5110->print_str("Into pH 4");
    lcd5110->set_cursor(3,0);
    lcd5110->print_str("Solution. Then");
    lcd5110->set_cursor(4,0);
    lcd5110->print_str("Press ENTER to");
    lcd5110->set_cursor(5,0);
    lcd5110->print_str("Start");
}


void three_point_cal(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;
    SENSOR_t *ph_sensor = (SENSOR_t *) SENSOR_ADDR;
    char count_buf[8];
    uint32_t adc_val = 0;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    snprintf(count_buf, 8, "0:%d", count--);

    lcd5110->clear();
    lcd5110->set_cursor(0,1);
    lcd5110->print_str("3 POINT CAL");
    lcd5110->set_cursor(1,5);
    if (point == 0)
        lcd5110->print_str("pH 7");
    else if (point == 1)
        lcd5110->print_str("pH 10");
    else
        lcd5110->print_str("pH 4");
    lcd5110->set_cursor(3,5);
    lcd5110->print_str(count_buf);

    adc_val = ph_sensor->read_samples();

    if (count != 0xFF)
        cal_buffer[count] = adc_val;

    if (count == 0xFF) {
        count = 30;

        uint32_t cal_avg = 0;

        for (uint8_t i=0; i<(uint8_t)COUNT_START; i++)
            cal_avg += cal_buffer[i];
        cal_avg /= (uint8_t) COUNT_START;

        if (point == 0) {
            point++;
            pH[0] = 7.0;
            mV[0] = ((float)cal_avg/4095.0)*3.3;
            event = EV_POINT_COMPLETE;
        }
        else if (point == 1) {
            point++;
            pH[1] = 10.0;
            mV[1] = ((float)cal_avg/4095.0)*3.3;
            event = EV_POINT_COMPLETE;
        }
        else {
            point = 0;
            pH[2] = 4.0;
            mV[2] = ((float)cal_avg/4095.0)*3.3;

            float pH_mean = 0.0;
            float mV_mean = 0.0;
            for (int i=0; i<3; i++) {
                pH_mean += pH[i];
                mV_mean += mV[i];
            }
            pH_mean /= 3.0;
            mV_mean /= 3.0;

            float Sxx = 0.0;
            float Syy = 0.0;
            float Sxy = 0.0;
            for (int i=0; i<3; i++) {
                Sxx += ((pH[i]-pH_mean)*(pH[i]-pH_mean));
                Syy += ((mV[i]-mV_mean)*(mV[i]-mV_mean));
                Sxy += ((pH[i]-pH_mean)*(mV[i]-mV_mean));
            }

            slope = Sxy/Sxx;
            offset = mV_mean - (slope*pH_mean);
            event = EV_CAL_COMPLETE;
        }
    }
}


void calibration_complete(void)
{
    static uint8_t i = 0;
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;

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








