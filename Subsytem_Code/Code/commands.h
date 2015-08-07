/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		commands.h
	*
	*	PURPOSE:	This program contains all includes and function prototypes required by commands.c
	*
	*	FILE REFERENCES:		can_api.h
	*
	*	EXTERNAL VARIABLES:		send_now, send_data, send_hk, send_coms
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:	None.
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	08/2/2015		Created.	
	*
*/

#include "can_api.h"
#include "can_lib.h"
#include "trans_lib.h"
#include <stdint.h>

/* Function Prototypes								 */	
void run_commands(void);
void send_response(void);
void send_housekeeping(void);
void send_sensor_data(void);
void send_coms_packet(void);
/*****************************************************/
