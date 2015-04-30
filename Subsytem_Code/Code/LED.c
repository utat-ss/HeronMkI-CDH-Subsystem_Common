/*
	Author: Keenan Burnett
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		LED.c
	*
	*	PURPOSE:	This program contains the basic API for setting LEDs.
	*
	*	FILE REFERENCES:	io.h, LED.h
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
	*	01/02/2015		Created.
	*
	*	02/06/2015		Edited the header.
	*
*/
#include <avr/io.h>
#include "LED.h"

void LED_Reg_Write(uint8_t val) {
	LEDPORT = ~val;
}

void LED_set(uint8_t LED) {

	LEDPORT &= ~(1 << LED);
}

void LED_clr(uint8_t LED) {
	LEDPORT |= (1 << LED);
}

void LED_toggle(uint8_t LED) {
	
	if(LED == 7)
	{
		PORTD ^= 0x01;
	}
	if(LED == 3)
	{
		PORTC ^= 0x01;
	}
	else
		LEDPORT ^= (1 << LED);
}

