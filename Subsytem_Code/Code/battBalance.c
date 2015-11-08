/*
	Author: Sam Murray

	***********************************************************************
	*	FILE NAME:		battBalance.c
	*
	*	PURPOSE:	This program contains the basic API for activiating battery balancing.
	*
	*	FILE REFERENCES:	battBalance.h
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
	*	2015/10/25		Created.
	*
	*	2015/11/08		Updated to be ready for physical testing
*/
#include "battBalance.h"

//if the flag is set from commands, then flip the pin that controls the battBalance on.
//batt balance pins are 23 and 24 

void run_battBalance()
{
	if (balance_l == 1)
	{
		PIN_set(BALANCE_L_P);
	}
	if (balance_l == 0)
	{
		PIN_clr(BALANCE_L_P);
	}
	if (balance_h == 1)
	{
		PIN_set(BALANCE_H_P);
	}
	if (balance_h == 0)
	{
		PIN_clr(BALANCE_H_P);
	}
	return;
};


