/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		dac_lib.h
	*
	*	PURPOSE:	This program contains includes and definitions related to dac_lib.c
	*	
	*	FILE REFERENCES:		config.h
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
	*	1/12/2016		Created.
	*
*/

#include "config.h"

void dac_initialize(void);
void dac_set(uint8_t* dac);
