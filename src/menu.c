/*
 * menu.c
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#include <lpc17xx_timer.h>

#include "menu.h"

char select = 0;

// lcd5110 refresh timer 1
static 	TIM_MATCHCFG_Type timer1_match0_cfg = {.MatchChannel = 0,
                                               .IntOnMatch = ENABLE,
                                               .StopOnMatch = DISABLE,
                                               .ResetOnMatch = ENABLE,
                                               .ExtMatchOutputType = TIM_EXTMATCH_NOTHING,
                                               .MatchValue = 2499};

void init_menu(void)
{
    TIM_ConfigMatch(LPC_TIM1, &timer1_match0_cfg);
    TIM_ResetCounter(LPC_TIM1);
    TIM_Cmd(LPC_TIM1, ENABLE);
}


void exit_menu(void)
{
    select = 0;
    TIM_Cmd(LPC_TIM1, DISABLE);
}


void main_menu(void)
{
	LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear(lcd5110);
    lcd5110->set_cursor(lcd5110, 0,3);
    lcd5110->print_str(lcd5110, "MAIN MENU");

    lcd5110->set_cursor(lcd5110, 2,3);
    if (select == 0) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "pH");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "pH");

    lcd5110->set_cursor(lcd5110, 3,3);
    if (select == 1) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "mV");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "mV");

    lcd5110->set_cursor(lcd5110, 4,3);
    if (select == 2) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "Cal");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "Cal");
}


void cal_menu(void)
{

	LCD5110_t *lcd5110 = (LCD5110_t *) LCD5110_ADDR;

    while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
    TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear(lcd5110);

    lcd5110->set_cursor(lcd5110, 1,2);
    if (select == 0) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "1 Point Cal");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "1 Point Cal");

    lcd5110->set_cursor(lcd5110, 2,2);
    if (select == 1) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "2 Point Cal");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "2 Point Cal");

    lcd5110->set_cursor(lcd5110, 3,2);
    if (select == 2) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "3 Point Cal");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "3 Point Cal");

    lcd5110->set_cursor(lcd5110, 4,2);
    if (select == 3) {
        lcd5110->set_inverse(lcd5110, 1);
        lcd5110->print_str(lcd5110, "Back");
        lcd5110->set_inverse(lcd5110, 0);
    }
    else
        lcd5110->print_str(lcd5110, "Back");
}

