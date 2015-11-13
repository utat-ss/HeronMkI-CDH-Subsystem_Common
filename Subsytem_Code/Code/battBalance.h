/*
	Author: Sam Murray
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		battBalance.h
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
	*	2015/10/25		Created.
	*
	*	2015/11/08		Updated to be ready for physical testing
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "port.h"
#include "global_var.h"

void run_battBalance(void);

