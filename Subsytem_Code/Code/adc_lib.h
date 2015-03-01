/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		adc_lib.h
	*
	*	PURPOSE:	This program contains includes and definitions related to adc_lib.c
	*
	*	FILE REFERENCES:
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
*/

#include "config.h"

/* ADC Multiplexer Register */
#define ADMUX_BASE			0x7C
// REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0

// REFS[1:0]: Chooses the reference voltage. [00] = AREF pin.
// ADLAR: ADC Left adjust register.
// MUX: Choice of Analog input channel. 00000 = ADC0.

/* ADC Special Register A */
#define ADCSRB_BASE			0x7B
// ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0

// ADEN: Set to enable the ADC.
// ADSC: ADC Start conversion bit.
// ADATE: Auto trigger enable bit.
// ADIF: ADC interrupt flag.
// ADIE: ADC interrupt enable bit.
// ADPS[2:0]: ADC prescaler selection bits.	[110] == 1/64

/* ADC Special Register B */
#define ADCSRA_BASE			0x7A
// ADHSM | ISRCEN | AFEREN | ADTS3 | ADTS2 | ADTS1 | ADTS0

// ADHSM: ADC High Speed Mode (if clock source > 200 kHz)
// ISRCEN: Current source enable. = 0 if using AREF pin.
// AREFEN: Analog Ref pin enable.
// ADTS: Auto-Trigger Selection bits. [0000] = Free Running Mode

/*ADC Data Register (low)*/
#define	ADCL_BASE			0x78

void adc_initialize(void);
void adc_read(uint8_t* array_ptr);

