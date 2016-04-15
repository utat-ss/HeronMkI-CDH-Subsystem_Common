/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		dac_lib.c
	*
	*	PURPOSE:	This program contains functions related to the digital-to-analog
	*				converter in the ATMEGA32M1.
	*
	*	FILE REFERENCES:		adc_lib.h
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
	*	1/12/2016		
	*
*/

#include "dac_lib.h"

/************************************************************************/
/*      DAC INITIALIZATION                                              */
/*																		*/
/*		This function will enable the DAC, and allow conversion to begin*/
/*		immediately. We are using AREF.									*/
/*		Output occurs automatically after DACH is written.				*/
/************************************************************************/
void dac_initialize(void)
{
	DACON = 0x03;
	return;
}

/************************************************************************/
/*		DAC SET															*/
/*																		*/
/*		This function takes a  value (0-1023) and then causes the		*/
/*		corresponding analog voltage to appear on the D2A pin.			*/
/*		Vout = dac * (Vref / 1023)										*/
/*		@parameter: *dac: This should be a uint8_t array[2] so that one */
/*		can pass all 10 bits. dac[0] = DACL, dac[1] = DACH.				*/
/************************************************************************/

void dac_set(uint8_t* dac)
{
	DACL = *dac;
	DACH = (*(dac + 1)) & 0x03;
	return;
}
