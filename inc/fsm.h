/*
 * fsm.h
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#ifndef FSM_H_
#define FSM_H_

#include "sensor.h"
#include "lcd5110.h"


typedef enum {
    EV_ANY,
    EV_NONE,
    EV_SELECT_PUSHED,
	EV_PH_SELECTED,
	EV_MV_SELECTED,
	EV_CAL_SELECTED,
	EV_BACK_SELECTED
} event_t;

extern event_t event;


typedef enum {
    ST_PH,
	ST_MV,
    ST_MAIN_MENU,
	ST_CAL_MENU
} state_t;


typedef struct state_machine FSM_t;

typedef struct state_machine {
	LCD5110_t *lcd5110;
    state_t current_state;
    void (*init_st) (void);
    void (*exit_st) (void);
    void (*function) (LCD5110_t *lcd5110);
    void (*run) (FSM_t* fsm);
} FSM_t;


void init_fsm(FSM_t *fsm, LCD5110_t *lcd5110);


#endif /* FSM_H_ */