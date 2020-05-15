#ifndef _STATE_MISSION_H_
#define _STATE_MISSION_H_ 

#include "state_types.h"

#include "state_ids.h"

STATE__T_STATE* STATE_MISSION_get_state(void);

STATE__E_STATE_ID* STATE_MISSION_get_transition_map(void);

STATE__E_ERROR STATE_MISSION_function(void);

#endif