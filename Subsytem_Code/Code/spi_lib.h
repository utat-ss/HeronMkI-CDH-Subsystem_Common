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
	*	4/30/2015		I got rid of the 'receive', and 'check' functions because 
	*					there was no need for them anymore.
	*
*/


//-----------FILE INCLUDES---------------
#include "config.h"
#include "port.h"
#include "Timer.h"
#include "global_var.h"
//---------------------------------------

//-----------CONSTANT DEFINES------------
#define	MCUCR_BASE	0x55
#define SPCR_BASE	0X4C
#define SPSR_BASE	0x4D
#define SPDR_BASE	0x4E

#define SPI_TIMEOUT	250				// Timeout for SPI polling operations.
#define SPI_SPSR_SPIF (0x1u << 7)		// SPIF flag in the SPSR register.

void spi_initialize_master(void);
uint8_t spi_transfer(uint8_t message);
uint8_t spi_transfer2(uint8_t message);
uint8_t spi_transfer3(uint8_t message);
uint8_t spi_transfer4(uint8_t message);
void spi_send_shunt_dpot_value(uint8_t message);
void spi_retrieve_temp(uint8_t* high, uint8_t* low);
void SS_set_high(void);
void SS_set_low(void);
void SS1_set_high(uint32_t);
void SS1_set_low(uint32_t);
void spi_retrieve_acc(uint8_t *high, uint8_t *low, uint8_t axis);
void spi_retrieve_humidity(uint8_t *high, uint8_t *low);
void pressure_sensor_init(uint8_t *pressure_calibration);
void spi_retrieve_pressure(uint8_t* arr);

