#ifndef _STATE_TYPES_H_
#define _STATE_TYPES_H_ 

#include "queue.h"

#include "state_ids.h"

#define C_STATE_MAX_ALLOWED_TRANSITIONS (3)


typedef enum _STATE__E_ERROR
{
	STATE_E_NO_ERROR = 0,

	STATE_E_STATE_NOT_FOUND_IN_MAP = 1,

	STATE_E_STATE_CHANGE_NOT_ALLOWED = 5,
	STATE_E_ALREADY_IN_THIS_STATE = 6,

	// Fatal error detector. Error code smaller is not a fatal error
	STATE_E_FATAL_ERROR_DETECTOR = 11, 
	
	/* Fatal Errors that must cause stop/reboot */
	STATE_E_UNATTENDED_TERMINATION_ERROR = 12,
	
}STATE__E_ERROR;


/**
 * Type for state callbacks
 */
typedef STATE__E_ERROR  (*STATE__T_EXECUTE_CALLBACK)(void);
typedef STATE__E_STATE_ID* (*STATE__T_GET_TRANSITION_MAP)(void);


typedef struct _STATE__T_STATE
{
	STATE__E_STATE_ID state_id;
	STATE__T_EXECUTE_CALLBACK   callback;
	STATE__T_GET_TRANSITION_MAP get_transition_map_callback;
}STATE__T_STATE;

#endif