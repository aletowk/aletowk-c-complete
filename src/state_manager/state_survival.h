#ifndef _STATE_SURVIVAL_H_
#define _STATE_SURVIVAL_H_ 

#include "state_types.h"

#include "state_ids.h"

STATE__T_STATE* STATE_SURVIVAL_get_state(void);

STATE__E_STATE_ID* STATE_SURVIVAL_get_transition_map(void);

STATE__E_ERROR STATE_SURVIVAL_function(void);

#endif