/*
	Author: Chris Zhang

	***********************************************************************
	*	FILE NAME:		comm_control.h
	*
	*	PURPOSE:	This program contains includes and definitions related to comm_control.c
	*
	*	FILE REFERENCES:		spi_lib.h trans_lib.h stdint.h stdbool.h
	*
	*	EXTERNAL VARIABLES:	
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:	
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	2/13/2016		Created.
	*
*/

#ifndef COMM_CONTROL_H
#define COMM_CONTROL_H

//port number for three spi devices
#include <stdint.h>
#include <stdbool.h>
#include "spi_lib.h"
#include "trans_lib.h"
#include "global_var.h"

#define VHFTSV 1
#define UHFTSV 2
#define TMPSNR 3
#define VHF_RECEIVE_MODE 0 // Low put the switch connect to receiver
#define VHF_TRANSMIT_MODE 1

//TEMPERARY PIN NUMBERS
#define TSV1_SS_PIN	1		//Transceiver 1 SS pin
#define TSV2_SS_PIN	2		//Transceiver 2 SS pin
#define SW1_ENABLE_PIN	3	//Switch 1 enable pin
#define SW2_ENABLE_PIN	4	//Switch 2 enable pin
#define TSV1_RESET_PIN	5	//Transceiver 1 reset pin
#define TSV2_RESET_PIN	6	//Transceiver 2 reset pin
#define SW1_PIN	7			//Switch 1 pin
#define SW2_PIN 8			//Switch 2 pin
#define TEM_SS_PIN	9		//Temperature sensor SS pin
#define RFFM_TR 10			//TR pin for RFFM6403

//Initialization for VHF transmit
void VHF_transmit_init();

//Initialization for VHF receive
void VHF_receive_init();

//Initialization for UHF transmit
void UHF_transmit_init();

//Initialization for UHF transmit
void UHF_receive_init();

//set certain ss to low
void set_coms_SS_low(uint8_t PIN);

//set certain ss to high
void set_coms_SS_high(uint8_t PIN);

uint8_t cmd_str_to_transceiver(uint8_t addr, uint8_t tsvNumber);
void transceiver_init2(uint8_t tsvNumber, bool isBeacon);
uint8_t spi_transfer_to_device(uint8_t message, uint8_t deviceNumber);
void reg_settings_UHF(uint8_t leave_on);
void reg_settings_VHF(uint8_t leave_on);
void reg_settings_UHF_Beacon(uint8_t leave_on);
void switchVHFset(bool mode);
void set_transceiver(uint8_t tsvNumber);
void switchUHFset(bool mode);


#endif