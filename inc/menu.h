/*
 * menu.h
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#ifndef MENU_H_
#define MENU_H_

#include "lcd5110.h"

extern char select;

void init_menu(void);
void exit_menu(void);
void main_menu(LCD5110_t *lcd5110);
void cal_menu(LCD5110_t *lcd5110);

#endif /* MENU_H_ */
