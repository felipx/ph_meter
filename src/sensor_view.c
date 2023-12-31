/*
 * sensor_view.c
 *
 *  Created on: Nov 13, 2023
 *      Author: felipe
 */


#include <stdint.h>
#include <stdio.h>
#include <lpc17xx_adc.h>
#include <lpc17xx_gpdma.h>
#include <lpc17xx_timer.h>

#include "sensor_view.h"
#include "sensor.h"
#include "calibration.h"


// ADC timer 0
static TIM_MATCHCFG_Type timer0_match1_cfg = {.MatchChannel = 1,
    	                                      .IntOnMatch = ENABLE,
                                              .StopOnMatch = DISABLE,
                                              .ResetOnMatch = ENABLE,
                                              .ExtMatchOutputType = TIM_EXTMATCH_TOGGLE,
                                              .MatchValue = 49};

// lcd5110 refresh timer 1
static 	TIM_MATCHCFG_Type timer1_match0_cfg = {.MatchChannel = 0,
                                               .IntOnMatch = ENABLE,
                                               .StopOnMatch = DISABLE,
                                               .ResetOnMatch = ENABLE,
                                               .ExtMatchOutputType = TIM_EXTMATCH_NOTHING,
                                               .MatchValue = 2499};


void init_sensor_view(void)
{
    TIM_ConfigMatch(LPC_TIM0, &timer0_match1_cfg);
    TIM_ResetCounter(LPC_TIM0);
    TIM_Cmd(LPC_TIM0, ENABLE);

    TIM_ConfigMatch(LPC_TIM1, &timer1_match0_cfg);
    TIM_ResetCounter(LPC_TIM1);
    TIM_Cmd(LPC_TIM1, ENABLE);

    GPDMA_ChannelCmd(0, ENABLE);
}


void exit_sensor_view(void)
{
    TIM_Cmd(LPC_TIM0, DISABLE);
    TIM_Cmd(LPC_TIM1, DISABLE);
    GPDMA_ChannelCmd(0, DISABLE);
}


void ph_view(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;
    SENSOR_t *ph_sensor = (SENSOR_t *) SENSOR_ADDR;

    char data_buf[8];
    char slope_buf[8];
    char offset_buf[8];
    float pH;
    uint32_t adc_val = 0;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    adc_val = ph_sensor->read_samples();

    pH = (((float)adc_val/4095)*3.3 - offset)/slope;

    if (pH < 0)
        pH = 0;

    snprintf(data_buf, 8, "%.1f", pH);
    snprintf(slope_buf, 8, "%.01f", slope*1000);
    snprintf(offset_buf, 8, "%.01f", offset);
    lcd5110->clear(lcd5110);
    lcd5110->set_cursor(lcd5110, 2,1);
    lcd5110->print_str(lcd5110, "pH = ");
    lcd5110->set_cursor(lcd5110, 1,6);
    lcd5110->print_big_str(lcd5110, data_buf);

    lcd5110->set_cursor(lcd5110, 4,0);
    lcd5110->print_str(lcd5110, "S= ");
    lcd5110->print_str(lcd5110, slope_buf);
    lcd5110->print_str(lcd5110, "mV/pH");

    lcd5110->set_cursor(lcd5110, 5,0);
    lcd5110->print_str(lcd5110, "off= ");
    lcd5110->print_str(lcd5110, offset_buf);
    lcd5110->print_str(lcd5110, "V");
}


void mv_view(void)
{
    LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;
    SENSOR_t *ph_sensor = (SENSOR_t *) SENSOR_ADDR;

    static float v_val =0.0;
    static float mv_val = 0.0;
    char v_buf[8];
    char mv_buf[8];
    uint32_t adc_val = 0;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    adc_val = ph_sensor->read_samples();

    v_val = ((float)adc_val/4095)*3.3;
    mv_val = ((v_val*0.66)/2 - 0.71)*1000;
    snprintf(v_buf, 8, "%.02f", v_val);
    snprintf(mv_buf, 8, "%.02f", mv_val);

    lcd5110->clear(lcd5110);
    lcd5110->set_cursor(lcd5110, 2,2);
    lcd5110->print_str(lcd5110, "V  = ");
    lcd5110->print_str(lcd5110, v_buf);
    lcd5110->set_cursor(lcd5110, 4,2);
    lcd5110->print_str(lcd5110, "mV = ");
    lcd5110->print_str(lcd5110, mv_buf);
}

