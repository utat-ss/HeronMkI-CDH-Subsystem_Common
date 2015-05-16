/*
	Author: Keenan Burnett
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		Timer.c
	*
	*	PURPOSE:	This program contains the basic API for using timers.
	*
	*	FILE REFERENCES:	io.h, interrupt.h, timer.h
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


#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"

ISR(TIMER1_COMPA_vect) {
	CTC_flag = 1;
}

// This function initializes a 16-bit timer used for delays.
void timer_init(void) {
	
	TIMSK1 = 0x00; //Disable timer interrupts
	TCCR1A = 0x00; //Timer not connected to any pins
	TCCR1B = 0x0A; //CTC mode; Timer_Rate = System_CLK/8 = 1MHz
	// 1 tick = 1 us (assume system clock = 8MHz)
}

// This is a blocking function that sets the timer value
// and waits until the value is reached. Waits for interrupt
// to execute.  Parameter "us" is truly 1 micro-second only
// if timer rate is set to 1MHz frequency. For example, this 
// project uses a system clock of 8MHz. Timer1 rate is set to
// System Clock divided by 8 which is 1MHz (in timer_init). 
// Therefore the "us" parameter truly is 1us.
void delay_us(uint16_t us) {
	CTC_flag = 0; //Clear CTC Interrupt flag
	TCNT1 = 0x0000; //Clear timer
	OCR1A = us; //Load compare value with desired delay in us
	TIMSK1 = 0x02; //Enable OCIE1A Interrupt
	while(!CTC_flag); //wait for the designated number of us
}

void delay_ms(uint16_t ms) {
	for(uint16_t i=0; i<ms; i++) {
		delay_us(1000);
	}
}

void delay_cycles(uint8_t cycles)	// @ 8 MHz, 1 cycle = 125 ns
{
	uint8_t i;
	
	for (i = 0; i < cycles; i++)
	{
		i = i;	// NOP
	}
	return;
}

