/*
	Author: Chris Zhang

	***********************************************************************
	*	FILE NAME:		comm_control.h
	*
	*	PURPOSE:	This program contains functions SPI devices on comms
	*
	*	FILE REFERENCES:		comm_control.h
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

#include "comm_control.h"

void transceiver_init2(uint8_t tsvNumber, bool isBeacon)
{
	/* SPI is already in MSB first, which is correct for the CC1120. */
	set_transceiver(tsvNumber);
	cmd_str(SRES);		//SRES			reset chip
	delay_ms(100);
	cmd_str(SFRX);		//SFRX          flush RX FIFO
	cmd_str(SFTX);      //SFTX          flush TX FIFO
	/* Settings taken from SmartRF */
	if(tsvNumber == VHFTSV)
		reg_settings_VHF(1);
	else if(tsvNumber == UHFTSV && isBeacon)
		reg_settings_UHF_Beacon(1);
	else if(tsvNumber == UHFTSV)
		reg_settings_UHF(1);
		
	/* Calibrate */
	cmd_str_to_transceiver(SCAL,tsvNumber);                   // Calibrate frequency synthesizer
	delay_ms(250);
	cmd_str_to_transceiver(SAFC,tsvNumber);					 // Automatic frequency control
	delay_ms(250);

	rx_mode = 1;
	tx_mode = 0;
	rx_length = 0;
	//prepareAck();
	/* Put In RX Mode */
	cmd_str(SRX);
	return;
}

//This was taken from 2016.1.30 Commit:4856111
// Note: set_transceiver(UHFTSV); should already have been called prior to using this function.
void reg_settings_UHF(uint8_t leave_on)
{
	reg_settings();
	if(!leave_on)
		set_transceiver(0);
	return;
}

//This is a duplicate of upper setting function. Need to modified to VHF latter.
// Note: set_transceiver(VHFTSV); should already have been called prior to using this function.
void reg_settings_VHF(uint8_t leave_on)
{
	reg_settings();
	if(!leave_on)
		set_transceiver(0);
	return;
}

// Note: set_transceiver(UHFTSV); should already have been called prior to using this function.
void reg_settings_UHF_Beacon(uint8_t leave_on)
{
	reg_settings();
	reg_write(MODCFG_DEV_E, 0b00011100); //Put the UHF transceiver to OOK
	if(!leave_on)
		set_transceiver(0);
	return;
}

uint8_t cmd_str_to_transceiver(uint8_t addr, uint8_t tsvNumber)
{
	uint8_t msg;
	set_transceiver(tsvNumber);
	msg = cmd_str(addr);
	set_transceiver(0);
	return msg;
}

uint8_t spi_transfer_to_device(uint8_t message, uint8_t deviceNumber)
{
	uint8_t receive_char;
	set_coms_SS_high(deviceNumber);
	receive_char = spi_transfer(message);
	set_coms_SS_low(deviceNumber);
	return receive_char;					// Transmission was successful, return the character that was received.
}


//Switch for VHF. 0 for receive. 1 for transmit
//Assume both switches turn on/off at the same time
void switchVHFset(bool mode)
{
	if(mode)
	{
		PIN_set(SW1_PIN);
		PIN_set(SW2_PIN);
	}
	else
	{
		PIN_clr(SW1_PIN);
		PIN_clr(SW2_PIN);
	}
}

void set_transceiver(uint8_t tsvNumber)
{
	current_transceiver = tsvNumber;
	set_coms_SS_high(TSV2_SS_PIN);
	set_coms_SS_high(TSV1_SS_PIN);
	delay_ms(25);
	if(tsvNumber == VHFTSV)
		set_coms_SS_low(TSV1_SS_PIN);
	if(tsvNumber == UHFTSV)
		set_coms_SS_low(TSV2_SS_PIN);
	delay_ms(25);
	return;
}

//Switch for UHF. 0 for receive. 1 for transmit
//Assume both switches turn on/off at the same time
void switchUHFset(bool mode){
	//if(mode)
//		PIN_set(RFFM_TR);
//	else
//		PIN_clr(RFFM_TR);
}