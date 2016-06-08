/*
	Author: Keenan Burnett, Sam Murray

	***********************************************************************
	*	FILE NAME:		port.c
	*
	*	PURPOSE:	This program contains the basic API for setting the micro pins.
	*
	*	FILE REFERENCES:	io.h, port.h
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
	*	11/08/2015		Transformed into port file and added control over many more pins
*/
#include <avr/io.h>
#include "port.h"

void LED_Reg_Write(uint8_t val) 
{
	PORTB = ~val;
	return;
}

void PIN_set(uint8_t PIN)
{
	if(PIN == 2)
	{
		PORTD |= 0b00001000;
	}
	if(PIN == 3)
	{
		PORTC |= 0b00000010;
	}
	if(PIN == 13)
	{
		PORTD |= 0b00100000;
	}
	if(PIN == 14)
	{
		PORTD |= 0b01000000;
	}
	if(PIN == 15)
	{
		PORTD |= 0b10000000;
	}
	if(PIN == 16)
	{
		PORTB |= 0b00000100;
	}
	if(PIN == 17)
	{
		PORTC |= 0b00010000;
	}
	if(PIN == 18)
	{
		PORTC |= 0b00100000;
	}
	if(PIN == 22)
	{
		PORTC |= 0b01000000;
	}
	if(PIN == 23)
	{
		PORTB |= 0b00001000;
	}
	if(PIN == 24)
	{
		PORTB |= 0b00010000;
	}
	if(PIN == 25)
	{
		PORTC |= 0b10000000;
	}
	if(PIN == 26)
	{
		PORTB |= 0b00100000;
	}
	if(PIN == 27)
	{
		PORTB |= 0b01000000;
	}
	if(PIN == 28)
	{
		PORTB |= 0b10000000;
	}
	if(PIN == 29)
	{
		PORTD |= 0b00000001;
	}
	if(PIN == 30)
	{
		PORTC |= 0b00000001;
	}
	if(PIN == 32)
	{
		PORTD |= 0b00000010;
	}
	return;
}

void PIN_toggle(uint8_t PIN)
{
	if(PIN == 2)
	{
		PORTD ^= 0b00001000;
	}
	if(PIN == 3)
	{
		PORTC ^= 0b00000010;
	}
	if(PIN == 13)
	{
		PORTD ^= 0b00100000;
	}
	if(PIN == 14)
	{
		PORTD ^= 0b01000000;
	}
	if(PIN == 15)
	{
		PORTD ^= 0b10000000;
	}
	if(PIN == 16)
	{
		PORTB ^= 0b00000100;
	}
	if(PIN == 17)
	{
		PORTC ^= 0b00010000;
	}
	if(PIN == 18)
	{
		PORTC ^= 0b00100000;
	}
	if(PIN == 22)
	{
		PORTC ^= 0b01000000;
	}
	if(PIN == 23)
	{
		PORTB ^= 0b00001000;
	}
	if(PIN == 24)
	{
		PORTB ^= 0b00010000;
	}
	if(PIN == 25)
	{
		PORTC ^= 0b10000000;
	}
	if(PIN == 26)
	{
		PORTB ^= 0b00100000;
	}
	if(PIN == 27)
	{
		PORTB ^= 0b01000000;
	}
	if(PIN == 28)
	{
		PORTB ^= 0b10000000;
	}
	if(PIN == 29)
	{
		PORTD ^= 0b00000001;
	}
	if(PIN == 30)
	{
		PORTC ^= 0b00000001;
	}
	if(PIN == 32)
	{
		PORTD ^= 0b00000010;
	}
	return;
}

void PIN_clr(uint8_t PIN)
{
	if(PIN == 2)
	{
		PORTD &= ~(0b00001000);
	}
	if(PIN == 3)
	{
		PORTC &= ~(0b00000010);
	}
	if(PIN == 13)
	{
		PORTD &= ~(0b00100000);
	}
	if(PIN == 14)
	{
		PORTD &= ~(0b01000000);
	}
	if(PIN == 15)
	{
		PORTD &= ~(0b10000000);
	}
	if(PIN == 16)
	{
		PORTB &= ~(0b00000100);
	}
	if(PIN == 17)
	{
		PORTC &= ~(0b00010000);
	}
	if(PIN == 18)
	{
		PORTC &= ~(0b00100000);
	}
	if(PIN == 22)
	{
		PORTC &= ~(0b01000000);
	}
	if(PIN == 23)
	{
		PORTB &= ~(0b00001000);
	}
	if(PIN == 24)
	{
		PORTB &= ~(0b00010000);
	}
	if(PIN == 25)
	{
		PORTC &= ~(0b10000000);
	}
	if(PIN == 26)
	{
		PORTB &= ~(0b00100000);
	}
	if(PIN == 27)
	{
		PORTB &= ~(0b01000000);
	}
	if(PIN == 28)
	{
		PORTB &= ~(0b10000000);
	}
	if(PIN == 29)
	{
		PORTD &= ~(0b00000001);
	}
	if(PIN == 30)
	{
		PORTC &= ~(0b00000001);
	}
	if(PIN == 32)
	{
		PORTD &= ~(0b00000010);
	}
	return;
}
