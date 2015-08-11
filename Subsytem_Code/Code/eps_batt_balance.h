/*
	Author: Sam Murray
	(Originally taken from Digi-Key Design Support code that was modified by Keenan Burnett)

	***********************************************************************
	*	FILE NAME:		eps_batt_balane.h
	*
	*	PURPOSE:	This file contains constant definitions and prototypes for eps_batt_balance.c
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
	*	07/26/2015		Created.
	*
*/
#include <avr/io.h>
#include "Timer.h"

#define DP1		3
#define DP2		2


void drive_set(uint8_t pin);
void drive_clr(uint8_t pin);


