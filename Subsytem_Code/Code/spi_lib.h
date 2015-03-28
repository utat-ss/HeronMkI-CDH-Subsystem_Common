/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		spi_lib.h
	*
	*	PURPOSE:	This program contains includes and definitions related to spi_lib.c
	*
	*	FILE REFERENCES:		config.h
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


//-----------FILE INCLUDES---------------
#include "config.h"
//---------------------------------------

//-----------CONSTANT DEFINES------------
#define	MCUCR_BASE	0x35
#define SPCR_BASE	0X2C
#define SPSR_BASE	0x2D
#define SPDR_BASE	0x2E

#define SPI_TIMEOUT	1000				// Timeout for SPI polling operations.
#define SPI_SPSR_SPIF (0x1u << 7)		// SPIF flag in the SPSR register.

void spi_initialize(void);
uint8_t spi_transfer(uint8_t message);
uint8_t spi_receive(uint8_t* r_message);
void spi_check(void);

