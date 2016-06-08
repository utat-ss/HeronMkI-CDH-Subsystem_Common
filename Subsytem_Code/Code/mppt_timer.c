/*
	Author: Sam Murray
	(Originally taken from Keenan's timer code)

	***********************************************************************
	*	FILE NAME:		mpptTimer.c
	*
	*	PURPOSE:	This program contains the basis for using the mppt function.
	*
	*	FILE REFERENCES:	mppttimer.h
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
	*	2015/11/08		Updated to be ready for physical testing
*/

#include "mppt_timer.h"


//Section to deal with timer interrupts
#if (SELF_ID == 1)
//When the A compare register is reached, turn on the MPPTX signal
ISR(TIMER0_COMPA_vect) {
	PIN_clr(MPPTX_P);
}

//When the B compare register is reached, turn on the MPPTY signal
ISR(TIMER0_COMPB_vect) {
	PIN_clr(MPPTY_P);
}

//When the timer overflows, turn off both signals
ISR(TIMER0_OVF_vect) {
	PIN_set(MPPTX_P);
	PIN_set(MPPTY_P);
}


//********************************************************************************
// Start of function section 
//********************************************************************************

// This function initializes a 8-bit timer used for generating PWM for mppt.
// NOTE: I cannot run this after global interrupts have been turned on
void mppt_timer_init(void) {
	
	TCNT0 = 0x0000; //Clear timer
	//For now, hard code in the values that the "duty cycle" registers will be set to. This will be updated later by the algorithm
	OCR0A = 0xC0; // b00111111
	OCR0B = 0xC0; // b10111111
	
	// Set WGM pins to 3 to try Fast PWM
	// Set COM0 pins to 1 1 for set on compare match and clear at top
	TCCR0A = 0b11000011; // COM0A1 | COM0A0 | COM0B1 | COM0B0 | - | - | WGM01 | WGM00
	TCCR0B = 0x01; // b00000001 // FOC0A | FOC0B | - | - | WGM02 | CS02 | CS01 | CS00  CS = 5 - pre-scale 1024 CS = 1 - no pre-scale
	//TIMSK0 = 0x07; // b00000111 Enable the A and B compare match interrupts. Also enable the timer overflow interrupt
	TCNT0 = 0x0000; //Clear timer
}

//This function will set the duty cycle of MPPTA
//Duty is an 8 bit number, so 0x7F is a duty cycle of 0.5
void set_duty_cycleX(uint8_t duty){
	OCR0A = duty;
}

//This function will set the duty cycle of MPPTB
void set_duty_cycleY(uint8_t duty){
	OCR0B = duty;
}

void run_mppt(void) {
	set_duty_cycleX(mpptx);
	set_duty_cycleY(mppty);	
}

#endif
