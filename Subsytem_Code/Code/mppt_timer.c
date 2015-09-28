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
#include "mppt_timer.h"

//When the A compare register is reached, turn on one of the LEDs
ISR(TIMER0_COMPA_vect) {
	//MPPTA_flag = 1;
	LED_clr(LED2);
}

//When the B compare register is reached, turn on one of the LEDs
ISR(TIMER0_COMPB_vect) {
	//MPPTB_flag = 1;
	LED_clr(LED3);
}

//When the timer overflows, turn off both LEDs
ISR(TIMER0_OVF_vect) {
	LED_set(LED2);
	LED_set(LED3);
	//MPPTA_flag = 0;
	//MPPTB_flag = 0;

}

// This function initializes a 8-bit timer used for generating PWM for mppt.
void mppt_timer_init(void) {
	
	TCNT0 = 0x0000; //Clear timer
	//For now, hard code in the values that the "duty cycle" registers will be set to. This will be updated later by the algorithm
	OCR0A = 0x3F; // b00111111
	OCR0B = 0xBF; // b10111111
	
	TCCR0A = 0x00; // b00000000 Don't connect any pins, but set to normal mode as a test
	TCCR0B = 0x05; // b00000101 Set clock pre-scaling to 1024 for now, will be 1 later for "fast" output
	TIMSK0 = 0x07; // b00000111 Enable the A and B compare match interrupts. Also enable the timer overflow interrupt
	
}

void mppt_timer(void) {
		
	
}

//This function will set the duty cycle of MPPTA
//Duty is an 8 bit number, so 0x7F is a duty cycle of 0.5
void set_duty_cycleA(uint8_t duty){
	OCR0A = duty;	
}

//This function will set the duty cycle of MPPTB
void set_duty_cycleB(uint8_t duty){
	OCR0B = duty;
}