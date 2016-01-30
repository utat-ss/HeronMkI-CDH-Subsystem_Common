/*
	Author: Sam Murray
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		batt_heater.h
	*
	*	PURPOSE:	This file contains constant definitions and prototypes for battBalance.c
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
	*	2016/01/25		Created.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "port.h" //This is for the pin definitions for the EPS system
#include "global_var.h"

void run_batt_heater(void);

