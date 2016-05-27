/*
	Author: Keenan Burnett, Sam Murray
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		port.h
	*
	*	PURPOSE:	This file contains constant definitions and prototypes for port.c
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
	*	11/08/2015		Merged LED with port files to add set/toggle/clear functionality for more pins 
*/
#include <avr/io.h>

#ifndef PIN_H_
#define PIN_H_

#define LED1 29
#define LED2 27
#define LED3 30

//FOR EPS
#define BALANCE_L_P	26
#define BALANCE_H_P	27
#define MPPTX_P		13
#define MPPTY_P		14
#define BATT_HEAT_P	16
#define DPOT_SS_P	32

#define Z_P			18
#define S0_P		22
#define S1_P		23
#define S2_P		24
#define S3_P		25



//FOR COMS
#define UHF_RST		14
#define VHF_RST		16
#define UHF_FE_EN	32
#define UHF_FE_TR	13
#define UHF_FE_BYP	18

//FOR PAY
#define EXP_RST		22

void LED_Reg_Write(uint8_t val);
void PIN_set(uint8_t PIN);
void PIN_clr(uint8_t PIN);
void PIN_toggle(uint8_t PIN);

#endif /* PIN_H_ */

