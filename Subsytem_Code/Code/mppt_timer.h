/*
	Author: Sam Murray
	(Originally taken from Keenan's timer code)

	***********************************************************************
	*	FILE NAME:		mpptTimer.h
	*
	*	PURPOSE:	This program contains the prototypes for mpptTimer.h
	*
	*	FILE REFERENCES:	None.
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
	*
*/

#include "LED.h"

void mppt_timer_init(void);
void mppt_timer(void);
void set_duty_cycleA(uint8_t duty);
void set_duty_cycleB(uint8_t duty);

