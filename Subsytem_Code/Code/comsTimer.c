/*
	Author: Keenan Burnett, Eric Elmoznino

	***********************************************************************
	*	FILE NAME:		comsTimer.c
	*
	*	PURPOSE:	This program contains the functions related to the coms timer.
	*
	*	FILE REFERENCES:	io.h, interrupt.h, mppttimer.h
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
	*	10/18/2015			Created.
	*
*/

#include "comsTimer.h"

#if (SELF_ID == 0)
//When the timer overflows, increment the time-count variables.
ISR(TIMER0_OVF_vect)
{
	// Counter being used for transceiver communications.
	count32ms++;
	// Increment variables here to keep track of time when an
	// "are you alive?" message is in progress
	if (REQUEST_ALIVE_IN_PROG)
		ISALIVE_COUNTER++;
	else 
		ISALIVE_COUNTER = 0;
}
#endif

void coms_timer_init(void)
{	
	count32ms = 0;
	TCNT0 = 0x0000; //Clear timer
	TCCR0A = 0x00; // b00000000 Don't connect any pins
	TCCR0B = 0x05; // 8MHz / 1024 is the clock frequency
	TIMSK0 = 0x01; // Overflow interrupt enabled. (Occurs when TCNT0 == 255)
	return;
}

