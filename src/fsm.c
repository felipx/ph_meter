/*
 * fsm.c
 *
 *  Created on: Oct 24, 2023
 *      Author: felipe
 */

#include <lpc17xx_timer.h>

#include "fsm.h"
#include "menu.h"

event_t event = EV_NONE;

typedef struct {
	state_t state;
	void (*init) (void);
	void (*exit) (void);
	void (*function) (LCD5110_t *lcd5110);
} state_function_t;


typedef struct {
    state_t state;
    event_t event;
    state_t next_state;
} matrix_row_t;


static state_function_t state_func_matrix[] = {
	// NAME        // INIT           // EXIT           // FUNC
    {ST_PH,        init_sensor_view, exit_sensor_view, ph_view  },
    {ST_MV,        init_sensor_view, exit_sensor_view, mv_view  },
	{ST_MAIN_MENU, init_menu,        exit_menu,        main_menu},
	{ST_CAL_MENU,  init_menu,        exit_menu,        cal_menu }
};


static matrix_row_t transition_matrix[] = {
    // CURR STATE  // EVENT          // NEXT STATE
    {ST_PH,        EV_SELECT_PUSHED, ST_MAIN_MENU},
	{ST_MV,        EV_SELECT_PUSHED, ST_MAIN_MENU},
    {ST_MAIN_MENU, EV_PH_SELECTED  , ST_PH       },
	{ST_MAIN_MENU, EV_MV_SELECTED  , ST_MV       },
	{ST_MAIN_MENU, EV_CAL_SELECTED , ST_CAL_MENU },
	{ST_CAL_MENU,  EV_BACK_SELECTED, ST_MAIN_MENU}
};


static void process_event(FSM_t *fsm)
{
	for (int i=0; i<(sizeof(transition_matrix)/sizeof(transition_matrix[0])); i++) {
		if ((fsm->current_state == transition_matrix[i].state) && (event == transition_matrix[i].event)) {
	        event = EV_NONE;
			fsm->exit_st();
			fsm->current_state = transition_matrix[i].next_state;
			fsm->init_st = state_func_matrix[fsm->current_state].init;
			fsm->exit_st = state_func_matrix[fsm->current_state].exit;
			fsm->function = state_func_matrix[fsm->current_state].function;
			fsm->init_st();
		    break;
		}
	}
}


static void run(FSM_t *fsm)
{
	while (1) {
	    process_event(fsm);
	    fsm->function(fsm->lcd5110);
	}
}


void init_fsm(FSM_t *fsm, LCD5110_t *lcd5110)
{
	fsm->lcd5110 = lcd5110;
	fsm->current_state = ST_PH;
	fsm->init_st = state_func_matrix[0].init;
	fsm->exit_st = state_func_matrix[0].exit;
	fsm->function = state_func_matrix[0].function;
	fsm->run = run;

	fsm->init_st();
}
