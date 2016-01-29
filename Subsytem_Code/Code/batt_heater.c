/*
	Author: Sam Murray

	***********************************************************************
	*	FILE NAME:		batt_heater.c
	*
	*	PURPOSE:	This program contains the basic API for activating the battery heater.
	*
	*	FILE REFERENCES:	batt_heater.h
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
#include "batt_heater.h"

//if the flag is set from commands, then flip the pin that controls the heater on.
//heater is pin 16

void run_batt_heater()
{
	if (batt_heater_control == 1)
	{
		PIN_set(BATT_HEAT_P);
	}
	if (batt_heater_control == 0)
	{
		PIN_clr(BATT_HEAT_P);
	}
	return;
};


