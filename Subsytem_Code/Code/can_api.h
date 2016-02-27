/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		can_api.h
	*
	*	PURPOSE:	This program contains prototypes and includes required for can_api.c
	*
	*	FILE REFERENCES:
	*
	*	EXTERNAL VARIABLES:	
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None yet.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:	
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	03/07/2015		Created.
	*
*/
#include "config.h"
#include "can_lib.h"
#include "global_var.h"
#include "trans_lib.h"
#include "commands.h"
#include "port.h"

/* Function Prototypes								 */	
void can_check_general(void);
void can_check_housekeep(void);
void can_send_message(uint8_t* data_array, uint8_t id);
void can_init_mobs(void);
void set_up_msg(uint8_t mailbox);
void clean_up_msg(uint8_t mailbox);
void decode_command(uint8_t* command_array);
/*****************************************************/

