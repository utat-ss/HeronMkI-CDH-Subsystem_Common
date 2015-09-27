/*
	Author: Keenan Burnett

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
	*	09/20/2015		All uses of LED7 --> LED1, all uses of LED6 --> LED2.
	*
*/
#include <avr/io.h>
#include "LED.h"

void LED_Reg_Write(uint8_t val) 
{
	LEDPORT = ~val;
	return;
}

void LED_set(uint8_t LED) 
{
	
	if(LED == 1)
	{
		PORTD &= ~(0x01);
	}
	if(LED == 2)
	{
		LEDPORT &= ~(1 << 6);
	}
	if(LED == 3)
	{
		PORTC &= ~(0x01);
	}
	return;
}

void LED_clr(uint8_t LED) 
{
	
	if(LED == 1)
	{
		PORTD |= 0x01;
	}
	if(LED == 2)
	{
		LEDPORT |= (1 << 6);
	}
	if(LED == 3)
	{
		PORTC |= 0x01;
	}
	return;
}

void LED_toggle(uint8_t LED) 
{
	if(LED == 1)
	{
		PORTD ^= 0x01;
	}
	if(LED == 2)
	{
		LEDPORT ^= (1 << 6);
	}
	if(LED == 3)
	{
		PORTC ^= 0x01;
	}
	return;
}

void LED_blink(uint8_t LED)
{
	LED_toggle(LED);
	delay_ms(25);
	LED_toggle(LED);
	delay_ms(25);
	return;
}

