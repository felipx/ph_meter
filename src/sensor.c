/*
 * sensor.c
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#include <stdio.h>
#include <lpc17xx_adc.h>
#include <lpc17xx_gpdma.h>
#include <lpc17xx_timer.h>

#include "sensor.h"

uint32_t *adc_ph_data = (uint32_t *) 0x2007C000;
uint32_t adc_ph_samples = 1024;

// adc timer 0
static TIM_TIMERCFG_Type timer0_cfg = {.PrescaleOption = TIM_PRESCALE_TICKVAL,
		                               .PrescaleValue = 250};

static TIM_MATCHCFG_Type timer0_match1_cfg = {.MatchChannel = 1,
		                                      .IntOnMatch = ENABLE,
											  .StopOnMatch = DISABLE,
											  .ResetOnMatch = ENABLE,
                                              .ExtMatchOutputType = TIM_EXTMATCH_TOGGLE,
											  .MatchValue = 49};

static uint32_t adc_val = 0;


void init_sensor_view(void)
{
	TIM_ConfigMatch(LPC_TIM0, &timer0_match1_cfg);
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE, &timer0_cfg);
	TIM_ResetCounter(LPC_TIM0);
	TIM_Cmd(LPC_TIM0, ENABLE);

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


void ph_view(LCD5110_t *lcd5110)
{
	char data_buf[5];
	adc_val = 0;

	while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

	for (int i=0; i<1024; i++)
		adc_val += (*(adc_ph_data + i) >> 4) & 0xFFF;
	adc_val /= 1024;

	snprintf(data_buf, 5, "%d", adc_val);
	lcd5110->clear();
	lcd5110->cursor(1,2);
	lcd5110->print_str("ADC = ");
	lcd5110->print_str(data_buf);

}


void mv_view(LCD5110_t *lcd5110)
{
	static float mv_val = 0;
	char data_buf[8];
	adc_val = 0;

	while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

	for (int i=0; i<1024; i++)
		adc_val += (*(adc_ph_data + i) >> 4) & 0xFFF;
	adc_val /= 1024;

    mv_val = ((float)adc_val/4095)*3.3;
	snprintf(data_buf, 8, "%.02f", mv_val);

	lcd5110->clear();
	lcd5110->cursor(2,2);
	lcd5110->print_str("V  = ");
	lcd5110->print_str(data_buf);
	lcd5110->cursor(4,2);
	lcd5110->print_str("mV = ");
	lcd5110->cursor(5,3);

}
