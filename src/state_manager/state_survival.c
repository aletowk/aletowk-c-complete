#include "state_survival.h"

#include <stdio.h>

#define MAX_STATE_SURVIVAL_COUNTER_VAL (12)



static STATE__T_STATE G_STATE_SURVIVAL_state = 
{
	.state_id = STATE_E_SURVIVAL,
	.callback = STATE_SURVIVAL_function,
	.get_transition_map_callback = STATE_SURVIVAL_get_transition_map
};

static unsigned char G_state_survival_counter = 0;

static STATE__E_STATE_ID G_STATE_SURVIVAL_transition_MAP[C_STATE_MAX_ALLOWED_TRANSITIONS] = 
					{
						STATE_E_MISSION,
						0,
						0
					};


STATE__T_STATE* STATE_SURVIVAL_get_state(void)
{
	return &G_STATE_SURVIVAL_state;
}

STATE__E_STATE_ID* STATE_SURVIVAL_get_transition_map(void)
{
	return G_STATE_SURVIVAL_transition_MAP;
}

STATE__E_ERROR STATE_SURVIVAL_function(void)
{
	printf("[STATE] Survival Mode execution...\n\r");
	G_state_survival_counter++;

	if(G_state_survival_counter >= MAX_STATE_SURVIVAL_COUNTER_VAL)
	{
		// return STATE_E_UNATTENDED_TERMINATION_ERROR;
	}
	
	return STATE_E_NO_ERROR;
}