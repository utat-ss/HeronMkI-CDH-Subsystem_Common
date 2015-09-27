/*
	Author: Sam Murray
	(Originally taken from Keenan's timer code)

	***********************************************************************
	*	FILE NAME:		mpptTimer.c
	*
	*	PURPOSE:	This program contains the basis for using the mppt function.
	*
	*	FILE REFERENCES:	io.h, interrupt.h, mppttimer.h
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
	*	09/26/2015		Created.
	*
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "mpptTimer.h"


//When the A compare register is reached, turn on one of the LEDs
ISR(TIMER0_COMPA_vect) {
	LED_set(LED3);
}

//When the B compare register is reached, turn on one of the LEDs
ISR(TIMER0_COMPB_vect) {
	LED_set(7);
}

//When the timer overflows, turn off both LEDs
ISR(TIMER0_OVF_vect) {
	LED_clr(LED3);
	LED_clr(7);
}

// This function initializes a 8-bit timer used for generating PWM for mppt.
void mpptTimer_init(void) {
	
	TIMSK0 = 0x07; //Enable the A and B compare match interrupts. Also enable the timer overflow interrupt
	TCCR0A = 0x03; //Don't connect any pins, but set to Fast PWM mode
	TCCR0B = 0x05; //Set the pre-scaling to 1024 for now, will be 1 later for "fast" output
	
	//For now, hardcode in the values that the "duty cycle" registers will be set to. This will be updated later by the algorithm
	OCR0A = 0x7F;
	OCR0B = 0x7F;
}

