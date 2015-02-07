/*
	Author: Keenan Burnett
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		Timer.h
	*
	*	PURPOSE:	This program contains the prototypes for Timer.h
	*
	*	FILE REFERENCES:	None.
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


#ifndef TIMER_H_
#define TIMER_H_

extern volatile uint8_t CTC_flag;

void timer_init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif /* TIMER_H_ */

