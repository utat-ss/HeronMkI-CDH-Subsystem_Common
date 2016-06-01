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
	//PIN_clr(LED2);
}

//When the B compare register is reached, turn on the MPPTY signal
ISR(TIMER0_COMPB_vect) {
	//PIN_clr(LED3);
}

//When the timer overflows, turn off both signals
ISR(TIMER0_OVF_vect) {
	//PIN_set(LED2);
	//PIN_set(LED3);
}


//********************************************************************************
// Start of function section 
//********************************************************************************

// This function initializes a 8-bit timer used for generating PWM for mppt.
// NOTE: I cannot run this after global interrupts have been turned on
void mppt_timer_init(void) {
	
	TCNT0 = 0x0000; //Clear timer
	//For now, hard code in the values that the "duty cycle" registers will be set to. This will be updated later by the algorithm
	OCR0A = 0x3F; // b00111111
	OCR0B = 0xBF; // b10111111
	
	TCCR0A = 0x00; // b00000000 Don't connect any pins, but set to normal mode as a test
	TCCR0B = 0x05; // b00000101 Set clock pre-scaling to 1024 for now, will be 1 later for "fast" output
	TIMSK0 = 0x07; // b00000111 Enable the A and B compare match interrupts. Also enable the timer overflow interrupt
	
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
