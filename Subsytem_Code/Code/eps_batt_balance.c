/*
	Author: Sam Murray
	(Originally taken from Digi-Key Design Support and modified by Keenan Burnett	)

	***********************************************************************
	*	FILE NAME:		eps_batt_balance.c
	*
	*	PURPOSE:	This program contains the basic API for running battery balancing.
	*
	*	FILE REFERENCES:	io.h, eps_batt_balance.h
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
	*	07/26/2015	Created.
	*
*/

#include "eps_batt_balance.h"


/************************************************************************/
/*	RUN BATTERY BALANCING							                    */
/*																		*/
/*	This funcion will run battery balancing for a set number of times   */
/*	For later operation, running or not running will be decided on by   */
/*	by the values read from the analog inputs of the battery voltages   */
/************************************************************************/
void run_batt_balance(void)
{
	uint8_t on_time = 2;
	uint8_t off_time = 3;
	
	drive_set(DP1);
	delay_us(on_time);
	
	drive_clr(DP1);
	delay_us(off_time);
}

/************************************************************************/
/*	SET AND CLEAR THE BATT BALANCE DRIVE PINS		                    */
/*																		*/
/*	The next two functions set the drive pins to high and low    		*/
/************************************************************************/

void drive_set(uint8_t pin) 
{
	PORTD &= ~(1 << pin);
}

void drive_clr(uint8_t pin) 
{
	PORTD |= (1 << pin);
}


