/*
	Author: Sam Murray
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		battBalance.h
	*
	*	PURPOSE:	This file contains constant definitions and prototypes for LED.c
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
	*	2015/10/25		Created.
	*
*/
#define LEDPORT PORTB

void PIN_Reg_Write(uint8_t val);
void PIN_set(uint8_t LED);
void PIN_clr(uint8_t LED);
void PIN_toggle(uint8_t LED);
void PIN_blink(uint8_t LED);

