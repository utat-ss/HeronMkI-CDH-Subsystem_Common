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
	*   01/30/2016      Changed to single conversion mode and added completion checking and error handling
	*
*/

#include "adc_lib.h"

/***************************************************************************/
/*      ADC INITIALIZATION                                                 */
/*																		   */
/*		This function will enable the ADC, and starts a single conversion. */
/*	    Prescaler is 1/8. We are using AREF.				               */
/*		The clock is at 1MHz, therefore high speed mode.				   */
/*		Interrupt are not enabled.										   */
/***************************************************************************/
void adc_initialize(void)
{
	uint8_t* ptr;
	ADMUX |= 0xC0;   // 0b1100 0000  REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0
	ptr = ADCSRA_BASE;		// Initialize ADCSRA 
	*ptr = 0b11010010;		// 0b1101 0010  ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0
	ptr = ADCSRB_BASE;		// Initialize ADCSRB
	*ptr = 0b10100000;		// 0b1010 0000  ADHSM | ISRCEN | AREFEN | - | ADTS3 | ADTS2 | ADTS1 | ADTS0
	
	#if (SELF_ID == 1)
	// For EPS we want REFS1 and REFS0 to be 00 instead of 11
	// We also want to use ADC 9 so MUX4-0 is 1001
	ADMUX = 0x49;   // 0b0000 0000  REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0
	
	//I think we want AREFEN on 
	//Trying with high speed, if I see issues, I'll also try low speed (ADPS = 110 for 1/64)
	ptr = ADCSRB_BASE;		// Initialize ADCSRB
	*ptr = 0b10000000;		// 0b0010 0000  ADHSM | ISRCEN | AREFEN | - | ADTS3 | ADTS2 | ADTS1 | ADTS0
	
	// We want single conversion mode, so ADATE = 0 and we don't write ADSC in the init. ADIF is cleared at 1
	// Set ADPS to 011 for a pre-scale of 8 on the sys clock to the ADC to get 1Mhz (set ADHSM to 1)
	// Set ADPS to 110 for a pre-scale of 64 on the sys clock to the ADC to get 125kHz (set ADHSM to 0)
	ptr = ADCSRA_BASE;		// Initialize ADCSRA
	*ptr = 0b11100010;		// 0b1001 0110  ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0
	#endif
	return;
}

/************************************************************************/
/*		ADC READING                                                     */
/*																		*/
/*		This function takes a pointer to an array of 2 bytes and loads	*/
/*		in the new sample from the ADC data registers in memory.        */
/*      Caps number of checks for completion to ~30 clock cycles       	*/
/*      adc_initialize and adc_set_pin must be called first.            */
/*      Returns -1 if error, else 0                                     */
/************************************************************************/

int adc_read(uint8_t* array_ptr)
{
	//delay_us(20);	//Length of time need for each conversion.
	uint8_t*	address = ADCL_BASE;
	uint8_t*	ptr = ADCSRA_BASE;		//Need to write ADSC high to start conversion. ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0
	
	//*ptr = *ptr | 0b01000000;

	//delay_ms(1);
	//if((*ptr & (0b01000000) ) == 1)
		//return -1;
	uint32_t counter = 0;
	while ( ((*ptr & (0b00010000) ) != 1) && counter < (30 * 64) ) { // while conversion not complete
		counter = counter + 1;
	}
	//
	//if (counter > 30)
	//{
		//uint8_t data[3];
		//data[0] = *((uint8_t*) ADCL_BASE);
		//data[1] = *((uint8_t*) ADCSRA_BASE);
		//data[2] = *((uint8_t*) ADCSRB_BASE);
		//errorREPORT(ADC_CONVERSION_ERROR, data);
		//return -1;
	//}
	
	*array_ptr = ADCL;				// Read the new sample.
	*(array_ptr + 1) = ADCH;
	
	return 0;
}

/************************************************************************/
/*		ADC SET PIN                                                     */
/*																		*/
/*		This function is for switching the input pin for the ADC.		*/
/*		Need to add a check later for ADC_num being between 1 and 10	*/
/************************************************************************/

void adc_set_pin(uint8_t ADC_num)
{
	
	if (ADC_num > 10)
		return;
	uint8_t* ptr;
	uint8_t x;
	ptr = ADMUX_BASE;		// Initialize ADMUX_BASE
	x = (ADC_num & 0b00001111); // force right-justified, external voltage reference
	*ptr = x;
	return;
}
