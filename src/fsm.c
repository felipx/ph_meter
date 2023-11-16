/*
 * fsm.c
 *
 *  Created on: Oct 24, 2023
 *      Author: felipe
 */

#include "fsm.h"
#include "calibration.h"
#include "init.h"
#include "menu.h"
#include "sensor_view.h"

event_t event = EV_NONE;

typedef struct {
    state_t state;
    void (*init) (void);
    void (*exit) (void);
    void (*function) ();
} state_function_t;


typedef struct {
    state_t state;
    event_t event;
    state_t next_state;
} matrix_row_t;


static state_function_t state_func_matrix[] = {
    // NAME           // INIT           // EXIT           // FUNC
    {ST_INIT,         init_init,        exit_init,        init                 },
    {ST_PH,           init_sensor_view, exit_sensor_view, ph_view              },
    {ST_MV,           init_sensor_view, exit_sensor_view, mv_view              },
    {ST_MAIN_MENU,    init_menu,        exit_menu,        main_menu            },
    {ST_CAL_MENU,     init_menu,        exit_menu,        cal_menu             },
    {ST_1P_CAL_START, init_cal_start,   exit_cal_start,   one_point_cal_start  },
    {ST_1P_CAL,       init_cal,         exit_cal,         one_point_cal        },
    {ST_2P_CAL_START, init_cal_start,   exit_cal_start,   two_point_cal_start  },
    {ST_2P_CAL,       init_cal,         exit_cal,         two_point_cal        },
    {ST_3P_CAL_START, init_cal_start,   exit_cal_start,   three_point_cal_start},
    {ST_3P_CAL,       init_cal,         exit_cal,         three_point_cal      },
    {ST_CAL_COMPLETE, init_cal,         exit_cal,         calibration_complete }
};


static matrix_row_t transition_matrix[] = {
    // CURR STATE     // EVENT             // NEXT STATE
    {ST_INIT,         EV_INIT_COMPLETE,    ST_PH          },
    {ST_PH,           EV_SELECT_PUSHED,    ST_MAIN_MENU   },
    {ST_MV,           EV_SELECT_PUSHED,    ST_MAIN_MENU   },
    {ST_MAIN_MENU,    EV_PH_SELECTED  ,    ST_PH          },
    {ST_MAIN_MENU,    EV_MV_SELECTED  ,    ST_MV          },
    {ST_MAIN_MENU,    EV_CAL_SELECTED ,    ST_CAL_MENU    },
    {ST_CAL_MENU,     EV_ONE_P_SELECTED,   ST_1P_CAL_START},
    {ST_CAL_MENU,     EV_TWO_P_SELECTED,   ST_2P_CAL_START},
    {ST_CAL_MENU,     EV_THREE_P_SELECTED, ST_3P_CAL_START},
    {ST_CAL_MENU,     EV_BACK_SELECTED,    ST_MAIN_MENU   },
    {ST_1P_CAL_START, EV_ABORT_CAL,        ST_CAL_MENU    },
    {ST_1P_CAL_START, EV_START_CAL,        ST_1P_CAL      },
    {ST_1P_CAL,       EV_ABORT_CAL,        ST_CAL_MENU    },
    {ST_1P_CAL,       EV_CAL_COMPLETE,     ST_CAL_COMPLETE},
    {ST_2P_CAL_START, EV_ABORT_CAL,        ST_CAL_MENU    },
    {ST_2P_CAL_START, EV_START_CAL,        ST_2P_CAL      },
    {ST_2P_CAL,       EV_ABORT_CAL,        ST_CAL_MENU    },
    {ST_2P_CAL,       EV_POINT_COMPLETE,   ST_2P_CAL_START},
    {ST_2P_CAL,       EV_CAL_COMPLETE,     ST_CAL_COMPLETE},
    {ST_3P_CAL_START, EV_ABORT_CAL,        ST_CAL_MENU    },
    {ST_3P_CAL_START, EV_START_CAL,        ST_3P_CAL      },
    {ST_3P_CAL,       EV_ABORT_CAL,        ST_CAL_MENU    },
    {ST_3P_CAL,       EV_POINT_COMPLETE,   ST_3P_CAL_START},
    {ST_3P_CAL,       EV_CAL_COMPLETE,     ST_CAL_COMPLETE},
    {ST_CAL_COMPLETE, EV_CAL_COMPLETE,     ST_PH          }
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
        fsm->function();
    }
}


void init_fsm(FSM_t *fsm)
{
    fsm->current_state = ST_INIT;
    fsm->init_st = state_func_matrix[0].init;
    fsm->exit_st = state_func_matrix[0].exit;
    fsm->function = state_func_matrix[0].function;
    fsm->run = run;

    fsm->init_st();
}
