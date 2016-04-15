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
#include "uart.h"
#include "Timer.h"
#include "global_var.h"
//---------------------------------------

//-----------CONSTANT DEFINES------------
#define	MCUCR_BASE		0x55
#define SPCR_BASE		0X4C
#define SPSR_BASE		0x4D
#define SPDR_BASE		0x4E

#define SPI_TIMEOUT		1000				// Timeout for SPI polling operations.
#define SPI_SPSR_SPIF	(0x1u << 7)		// SPIF flag in the SPSR register.

/* PRESSURE SENSOR DEFINITIONS */
#define PROM_READ				0xA0
#define PRES_RESET				0x1E
#define CONVERT_PRES_OSR_256	0x40
#define CONVERT_PRES_OSR_4096	0x48
#define CONVERT_TEMP_OSR_256	0x50
#define CONVERT_TEMP_OSR_4096	0x58
#define ADC_READ				0x00

/* SPI CHIP SELECT DEFINITIONS */
#define COMS_TEMP_SS			0
#define COMS_UHF_SS				1
#define COMS_VHF_SS				2
#define EPS_TEMP_CS				3
#define EPS_DPOT_CS				4
#define PAY_EXP1_CS				5
#define PAY_HEATER1_CS			6
#define PAY_HEATER2_CS			7
#define PAY_HEATER3_CS			8
#define PAY_HEATER4_CS			9
#define PAY_HEATER5_CS			10
#define PAY_VALVE_1n_CS			11
#define PAY_VALVE_1p_CS			12
#define PAY_VALVE_2n_CS			13
#define PAY_VALVE_2p_CS			14
#define PAY_VALVE_3n_CS			15
#define PAY_VALVE_3p_CS			16
#define PAY_VALVE_4n_CS			17
#define PAY_VALVE_4p_CS			18
#define PAY_ACCEL_CS			19
#define PAY_PRESL_CS			20
#define PAY_PRESH_CS			21
#define PAY_HUM_CS				22
#define PAY_TEMP0_CS			23
#define PAY_TEMP1_CS			24
#define PAY_TEMP2_CS			25
#define PAY_TEMP3_Cs			26
#define PAY_TEMP3_Cs			27

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
void pressure_sensor_init(uint8_t* pressure_calibration);
uint32_t spi_retrieve_pressure(void);
uint32_t spi_retrieve_pressure_temp(void);

