#include "state_manager.h"
#include "./state_ids.h"
#include "./state_survival.h"
#include "./state_mission.h"
#include <stdio.h>

#include <pthread.h>
#include <unistd.h>
#include "common.h"

#define C_STATE_MANAGER_DEFAULT_MODE (STATE_E_SURVIVAL)
#define C_STATE_MANAGER_DEFAULT_MODE_CALLBACK (STATE_SURVIVAL_function)
#define C_STATE_MANAGER_DEFAULT_MODE_GET_TRANSITIONS_CALLBACK (STATE_SURVIVAL_get_transition_map)

#define C_STATE_MANAGER_MAX_STATES (2)

static STATE__T_STATE_MANAGER  G_STATE_MANAGER_handler = 
			{
				.current_state = {.state_id = C_STATE_MANAGER_DEFAULT_MODE,
								  .callback = C_STATE_MANAGER_DEFAULT_MODE_CALLBACK,
								  .get_transition_map_callback = C_STATE_MANAGER_DEFAULT_MODE_GET_TRANSITIONS_CALLBACK
								},
				.read_queue = NULL,
				.write_queue = NULL
			};

static STATE__T_STATE  G_STATE_MANAGER_state_map[C_STATE_MANAGER_MAX_STATES];

static STATE__E_ERROR STATE_MANAGER__initialization(QUEUE* comm_queue);
static STATE__E_ERROR STATE_MANAGER__update(void);
static STATE__E_ERROR STATE_MANAGER__execution_loop(void);
static STATE__E_ERROR STATE_MANAGE__change_state(STATE__E_STATE_ID target_state);

static void state_command_sender_simulator(void);

#define COMM_QUEUE_SIZE (3)
#define COMM_QUEUE_ITEM_SIZE (3)

void* test_state_manager_thread(void* args)
{
	QUEUE * comm_queue = NULL;	
	STATE__E_ERROR ret_state = STATE_E_NO_ERROR;
	char ret_queue = 0;
	
	(void) args;
	printf("[STATE MANAGER THREAD] Test State Manager !\n\r");

	comm_queue = QUEUE_get_my_queue();
	ret_queue = create_queue(comm_queue, 3, 3);
	printf("[STATE MANAGER THREAD] create_queue return :%d\n\r",ret_queue);

	ret_state = STATE_MANAGER__initialization(comm_queue);
	if(ret_state != STATE_E_NO_ERROR)
	{
		printf("[STATE MANAGER THREAD] Error while STATE_MANAGER__initialization code : %d\n\r",ret_state);
		pthread_cancel(pthread_self());
	}

	while(1)
	{
		ret_state = STATE_MANAGER__update();
		if(ret_state != STATE_E_NO_ERROR)
		{
			printf("[STATE] Error while STATE_MANAGER__update code %d\n\r",ret_state);
			if(ret_state > STATE_E_FATAL_ERROR_DETECTOR)
			{
				pthread_cancel(pthread_self());
			}
		}
		else
		{
			/* Here add acknowledge to the origin of the change state request */
		}
		ret_state = STATE_MANAGER__execution_loop();
		if(ret_state != STATE_E_NO_ERROR)
		{
			printf("[STATE MANAGER THREAD] Error while execution state %d callback code : %d\n",G_STATE_MANAGER_handler.current_state.state_id,ret_state);
			if(ret_state > STATE_E_FATAL_ERROR_DETECTOR)
			{
				pthread_cancel(pthread_self());
			}
		}
		sleep(C_STATE_SLEEP_PERIOD_S);
	}
}

static STATE__E_ERROR STATE_MANAGER__initialization(QUEUE* comm_queue)
{
	G_STATE_MANAGER_handler.current_state.state_id = C_STATE_MANAGER_DEFAULT_MODE;
	G_STATE_MANAGER_handler.current_state.callback = C_STATE_MANAGER_DEFAULT_MODE_CALLBACK;
	G_STATE_MANAGER_handler.current_state.get_transition_map_callback = C_STATE_MANAGER_DEFAULT_MODE_GET_TRANSITIONS_CALLBACK;
	G_STATE_MANAGER_handler.read_queue = comm_queue;
	G_STATE_MANAGER_handler.write_queue = comm_queue;

	G_STATE_MANAGER_state_map[0] = *STATE_SURVIVAL_get_state();
	G_STATE_MANAGER_state_map[1] = *STATE_MISSION_get_state();

	return STATE_E_NO_ERROR;
}

static STATE__E_ERROR STATE_MANAGER__execution_loop(void)
{
	return G_STATE_MANAGER_handler.current_state.callback();
}


static STATE__E_ERROR STATE_MANAGER__update(void)
{
	unsigned char comm_buffer[COMM_QUEUE_SIZE] = {0};
	int ret_int = 0;
	STATE__E_ERROR ret_state = STATE_E_NO_ERROR;

	ret_int = queue_receive(G_STATE_MANAGER_handler.read_queue,comm_buffer);
	if(ret_int == 0)
	{
		switch(comm_buffer[0])
		{
			case STATE_E_COMMAND_CHANGE_STATE:
			{
				ret_state = STATE_MANAGE__change_state(comm_buffer[1]);
				break;
			}
			case STATE_E_COMMAND_EMPTY:
			default:
				ret_state = STATE_E_NO_ERROR;
				break;	
		}
	}
	return ret_state;
}

static int STATE_MANAGER__isTransitionAllowed(STATE__E_STATE_ID target_state)
{
	unsigned int i = 0;
	STATE__E_STATE_ID * transition_map = G_STATE_MANAGER_handler.current_state.get_transition_map_callback();
	for(i = 0; i < C_STATE_MAX_ALLOWED_TRANSITIONS; i++)
	{
		if(transition_map[i] == target_state)
		{
			return 1;
		}
	}
	return 0;
}

static STATE__E_ERROR STATE_MANAGER__get_state_in_map(STATE__T_STATE* _state, STATE__E_STATE_ID target_state)
{
	unsigned int i = 0;
	for(i = 0 ; i < C_STATE_MANAGER_MAX_STATES;i++)
	{
		if(G_STATE_MANAGER_state_map[i].state_id == target_state)
		{
			*_state = G_STATE_MANAGER_state_map[i];
			return STATE_E_NO_ERROR;
		}
	}
	return STATE_E_STATE_NOT_FOUND_IN_MAP;
}

static STATE__E_ERROR STATE_MANAGE__change_state(STATE__E_STATE_ID target_state)
{
	STATE__T_STATE new_state;
	STATE__E_ERROR ret_state = STATE_E_NO_ERROR;

	printf("[STATE] Change demand: Current state : %d\n\r",G_STATE_MANAGER_handler.current_state.state_id);
	printf("                       Targer  state : %d\n\r",target_state);
	
	if(G_STATE_MANAGER_handler.current_state.state_id == target_state)
	{
		return STATE_E_ALREADY_IN_THIS_STATE;
	}
	else
	{
		if(STATE_MANAGER__isTransitionAllowed(target_state))
		{
			ret_state = STATE_MANAGER__get_state_in_map(&new_state,target_state);
			if(ret_state == 0)
			{
				G_STATE_MANAGER_handler.current_state = new_state;
				return STATE_E_NO_ERROR;
			}
			else
			{
				return ret_state;
			}
		}
		else
		{
			return STATE_E_STATE_CHANGE_NOT_ALLOWED;
		}
	}
}