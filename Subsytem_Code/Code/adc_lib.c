/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		adc_lib.c
	*
	*	PURPOSE:	This program contains functions related to the analog-to-digital
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
	*	2/28/2015		Created.
	*
	*	3/28/2015		I added code to adc_initialize in order to change the
	*					selected ADC pin from ADC0 to ADC2. After breadboarding
	*					the ATMEGA32M1 I am now using pin PE2 for an external clock.
	*	09/27/2015		Sam added code to change the input pin for the ADC to use with the MPPT code
	*
*/

#include "adc_lib.h"

/************************************************************************/
/*      ADC INITIALIZATION                                              */
/*																		*/
/*		This function will enable the ADC, and allow conversion to begin*/
/*		immediately. Prescaler is 1/8. We are using AREF.				*/
/*		The clock is at 1MHz, therefore high speed mode.				*/
/*		The ADC is now in "free running mode."							*/
/*		Interrupt are not enabled.										*/
/************************************************************************/
void adc_initialize(void)
{
	uint8_t* ptr;
	uint8_t x;
	
	ptr = ADMUX_BASE;		// Initialize ADMUX_BASE
	x = 0b00000010;			// Choose ADC2
	*ptr = x;
	
	ptr = ADCSRA_BASE;		// Initialize ADCSRA
	x = 0b11100010;
	*ptr = x;
		
	ptr = ADCSRB_BASE;		// Initialize ADCSRB
	x = 0b10100000;
	*ptr = x;
	
	return;
}

/************************************************************************/
/*		ADC READING                                                     */
/*																		*/
/*		This function takes a pointer to an array of 2 bytes and loads	*/
/*		in the new sample from the ADC data registers in memory.		
/************************************************************************/

void adc_read(uint8_t* array_ptr)
{
	//delay_us(20);	//Length of time need for each conversion.
	uint8_t*	address = ADCL_BASE;
	
	*array_ptr = *address;				// Read the new sample.
	*(array_ptr + 1) = *(address + 1);
	
	return;
}

/************************************************************************/
/*		ADC SET PIN                                                     */
/*																		*/
/*		This function is for switching the input pin for the ADC.		*/
/*		Need to add a check later for ADC_num being between 1 and 10	*/
/************************************************************************/

void adc_set_pin(uint8_t ADC_num)
{
	uint8_t* ptr;
	uint8_t x;
	
	ptr = ADMUX_BASE;		// Initialize ADMUX_BASE
	x = ADC_num;
	*ptr = x;
	return;
}
