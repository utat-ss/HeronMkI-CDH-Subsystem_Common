/*
	Author: Keenan Burnett
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		LED.h
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
	*	01/02/2015		Created.
	*
	*	02/06/2015		Edited the header.
	*
*/
#ifndef LED_H_
#define LED_H_

#define LEDPORT PORTB
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7

void LED_Reg_Write(uint8_t val);
void LED_set(uint8_t LED);
void LED_clr(uint8_t LED);
void LED_toggle(uint8_t LED);



#endif /* LED_H_ */

