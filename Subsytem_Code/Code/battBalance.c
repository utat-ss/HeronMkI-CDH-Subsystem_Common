/*
	Author: Sam Murray

	***********************************************************************
	*	FILE NAME:		battBalance.c
	*
	*	PURPOSE:	This program contains the basic API for activiating battery balancing.
	*
	*	FILE REFERENCES:	io.h, battBalance.h
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
#include <avr/io.h>
#include "battBalance.h"

void PIN_Reg_Write(uint8_t val) 
{
	LEDPORT = ~val;
	return;
}

void PIN_set(uint8_t PIN) 
{
	
	if(PIN == 23)
	{
		PORTB |= 0x08; //binary 00001000
	}
	if(PIN == 24)
	{
		PORTB |= 0x16;  //binary 00010000
	}
	return;
}

void PIN_clr(uint8_t PIN) 
{
	
	if(PIN == 23)
	{
		LEDPORT |= (1 << 3);
	}
	if(PIN == 24)
	{
		LEDPORT |= (1 << 4);
	}
	return;
}

void PIN_toggle(uint8_t PIN) 
{
	if(PIN == 23)
	{
		LEDPORT ^= (1 << 3);
	}
	if(PIN == 24)
	{
		LEDPORT ^= (1 << 4);
	}
	return;
}

void PIN_blink(uint8_t PIN)
{
	PIN_toggle(PIN);
	delay_ms(25);
	PIN_toggle(PIN);
	delay_ms(25);
	return;
}

