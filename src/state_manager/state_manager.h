#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_ 

#include "state_types.h"

#define C_STATE_SLEEP_PERIOD_S (1)


/* In message to state manager we have : */
/* [cmdCode  |  cmdArg1 | cmdArg2] */
typedef enum _STATE__E_COMMAND_CODE
{
	STATE_E_COMMAND_EMPTY = 0,
	STATE_E_COMMAND_CHANGE_STATE = 1,
}STATE__E_COMMAND_CODE;

typedef struct _STATE__T_STATE_MANAGER
{
	STATE__T_STATE current_state;
	STATE__T_STATE * state_map;
	QUEUE * read_queue;
	QUEUE * write_queue;
}STATE__T_STATE_MANAGER;

void* test_state_manager_thread(void* args);

#endif