/*
	Author: Sam Murray
	(Originally taken from Keenan's timer code)

	***********************************************************************
	*	FILE NAME:		mpptTimer.h
	*
	*	PURPOSE:	This program contains the prototypes for mpptTimer.h
	*
	*	FILE REFERENCES:	io.h, interrupt.h, port.h, can_api.h
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
	*	09/26/2015		Created.
	*
	*	2015/11/08		Updated function names
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "port.h"
#include "global_var.h"

void mppt_timer_init(void);
void run_mppt(void);
void set_duty_cycleX(uint8_t duty);
void set_duty_cycleY(uint8_t duty);

