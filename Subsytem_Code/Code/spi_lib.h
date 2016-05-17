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

#ifndef SPI_LIB_H
#define SPI_LIB_H
//-----------FILE INCLUDES---------------
#include "config.h"
#include "port.h"
#include "uart.h"
#include "Timer.h"
#include "global_var.h"
#if	(SELF_ID == 2)
#include "port_expander.h"
#endif
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

/* ACCELEROMETER DEFINITIONS */
#define ACC_WRITE				0x0A
#define ACC_READ				0x0B
#define ACC_READ_FIFO			0x0D	
#define POWER_CTL				0x2D	
#define XDATAL					0x0E
#define XDATAH					0x0F
#define YDATAL					0x10
#define YDATAH					0x11
#define ZDATAL					0x12
#define ZDATAH					0x13	

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
#define PAY_VALVE_1A_CS			11
#define PAY_VALVE_1B_CS			12
#define PAY_VALVE_2A_CS			13
#define PAY_VALVE_2B_CS			14
#define PAY_VALVE_3A_CS			15
#define PAY_VALVE_3B_CS			16
#define PAY_VALVE_4A_CS			17
#define PAY_VALVE_4B_CS			18
#define PAY_PRESL_CS			19
#define PAY_PRESH_CS			20
#define PAY_ACCEL_CS			21
#define PAY_TEMP_CS				22
#define PAY_HUM_CS				23
#define PAY_TEMP0_CS			24
#define PAY_TEMP1_CS			25
#define PAY_TEMP2_CS			26
#define PAY_TEMP3_CS			27
#define PAY_TEMP4_CS			28
#define ADC1_CS					29
#define ADC2_CS					30
#define ADC3_CS					31
#define ADC_FL_CS				32

/* SPI CHIP SELECT PIN DEFINITIONS */
#define COMS_TEMP_PIN			14
#define COMS_UHF_PIN			29
#define COMS_VHF_PIN			27
#define EPS_TEMP_PIN			17
#define EPS_DPOT_PIN			32
#define PAY_EXP1_PIN			32



void spi_initialize_master(void);
uint8_t spi_transfer(uint8_t message);
uint8_t spi_transfer2(uint8_t message);
uint8_t spi_transfer3(uint8_t message);
uint8_t spi_transfer4(uint8_t message);
void spi_send_shunt_dpot_value(uint8_t message);
uint16_t spi_retrieve_temp(uint8_t chip_select);
void SS_set_high(void);
void SS_set_low(void);
void SS1_set_high(uint32_t);
void SS1_set_low(uint32_t);
uint16_t spi_retrieve_acc(uint8_t axis)
void spi_retrieve_humidity(uint8_t *high, uint8_t *low);
void pressure_sensor_init(uint8_t* pressure_calibration);
uint32_t spi_retrieve_pressure(void);
uint32_t spi_retrieve_pressure_temp(void);
void accelerometer_init(void);
void initialize_adc_all(void);
uint16_t spi_retrieve_adc_reading(uint8_t adc, uint8_t channel);

#endif
