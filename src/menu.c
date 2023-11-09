/*
 * menu.c
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#include <lpc17xx_timer.h>

#include "menu.h"

char select = 0;

void init_menu(void)
{
	TIM_ResetCounter(LPC_TIM1);
	TIM_Cmd(LPC_TIM1, ENABLE);
}


void exit_menu(void)
{
	select = 0;
	TIM_Cmd(LPC_TIM1, DISABLE);
}


void main_menu(LCD5110_t *lcd5110)
{
	while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();
	lcd5110->cursor(0,3);
	lcd5110->print_str("MAIN MENU");

	lcd5110->cursor(2,3);
	if (select == 0) {
		lcd5110->set_inverse(1);
	    lcd5110->print_str("pH");
	    lcd5110->set_inverse(0);
	}
	else
		lcd5110->print_str("pH");

	lcd5110->cursor(3,3);
	if (select == 1) {
		lcd5110->set_inverse(1);
        lcd5110->print_str("mV");
        lcd5110->set_inverse(0);
	}
	else
		lcd5110->print_str("mV");

	lcd5110->cursor(4,3);
    if (select == 2) {
    	lcd5110->set_inverse(1);
    	lcd5110->print_str("Cal");
    	lcd5110->set_inverse(0);
    }
    else
        lcd5110->print_str("Cal");
}


void cal_menu(LCD5110_t *lcd5110)
{
	while (!TIM_GetIntStatus(LPC_TIM1, TIM_MR0_INT));
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);

    lcd5110->clear();

	lcd5110->cursor(1,2);
	if (select == 0) {
		lcd5110->set_inverse(1);
	    lcd5110->print_str("1 Point Cal");
	    lcd5110->set_inverse(0);
	}
	else
		lcd5110->print_str("1 Point Cal");

	lcd5110->cursor(2,2);
	if (select == 1) {
		lcd5110->set_inverse(1);
        lcd5110->print_str("2 Point Cal");
        lcd5110->set_inverse(0);
	}
	else
		lcd5110->print_str("2 Point Cal");

	lcd5110->cursor(3,2);
    if (select == 2) {
    	lcd5110->set_inverse(1);
    	lcd5110->print_str("3 Point Cal");
    	lcd5110->set_inverse(0);
    }
    else
        lcd5110->print_str("3 Point Cal");

    lcd5110->cursor(4,2);
    if (select == 3) {
    	lcd5110->set_inverse(1);
    	lcd5110->print_str("Back");
    	lcd5110->set_inverse(0);
    }
    else
        lcd5110->print_str("Back");
}

