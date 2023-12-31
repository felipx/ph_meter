/*
 * fsm.h
 *
 *  Created on: Oct 25, 2023
 *      Author: felipe
 */

#ifndef FSM_H_
#define FSM_H_


typedef enum {
    EV_ANY,
    EV_NONE,
	EV_INIT_COMPLETE,
    EV_SELECT_PUSHED,
    EV_PH_SELECTED,
    EV_MV_SELECTED,
    EV_CAL_SELECTED,
    EV_BACK_SELECTED,
    EV_ONE_P_SELECTED,
    EV_TWO_P_SELECTED,
    EV_THREE_P_SELECTED,
    EV_ABORT_CAL,
    EV_START_CAL,
    EV_POINT_COMPLETE,
    EV_CAL_COMPLETE
} event_t;

extern event_t event;


typedef enum {
	ST_INIT,
    ST_PH,
    ST_MV,
    ST_MAIN_MENU,
    ST_CAL_MENU,
    ST_1P_CAL_START,
    ST_1P_CAL,
    ST_2P_CAL_START,
    ST_2P_CAL,
    ST_3P_CAL_START,
    ST_3P_CAL,
    ST_CAL_COMPLETE
} state_t;


typedef struct state_machine FSM_t;

typedef struct state_machine {
    state_t current_state;
    void (*init_st) (void);
    void (*exit_st) (void);
    void (*function) (void);
    void (*run) (FSM_t* fsm);
} FSM_t;


void init_fsm(FSM_t *fsm);


#endif /* FSM_H_ */
