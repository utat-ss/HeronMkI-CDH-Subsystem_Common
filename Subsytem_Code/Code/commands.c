/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		commands.c
	*
	*	PURPOSE:	This program contains all the commands which can be acted upon by the SSM.
	*
	*				The reason why these commands are not executed within within decode_command()
	*				in can_api.c is purely for organizational convenience.
	*
	*	FILE REFERENCES:		commands.h
	*
	*	EXTERNAL VARIABLES:		send_now, send_data, send_hk, send_coms
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:	None.
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	08/2/2015		Created.	
	*
*/

#include "commands.h"

void run_commands(void)
{
	if (send_now == 1)
		send_response();
	if (send_hk == 1)
		send_housekeeping();
	if (send_data == 1)
		send_sensor_data();
	if (send_coms == 1)
		send_coms_packet();
	return;	
}
/*****************************************************/

void send_response(void)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		send_arr[i] = 0xF0;		// Message to be sent back to the OBC.
	}
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	send_hk = 0;
	return;
}

void send_housekeeping(void)
{	
	uint8_t i;
		
	for (i = 0; i < 8; i ++)
	{
		send_arr[i] = 0xF0;		// Message to be sent back to the OBC.
	}
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	send_hk = 0;
	return;
}

void send_sensor_data(void)
{
	uint8_t i, high, low;
	
	for (i = 0; i < 8; i ++)
	{
		send_arr[i] = 0x00;		// Message to be sent back to the OBC.
	}
			
	//adc_read(&send_arr[0]);	// This line was used to acquire temp from an analog sensor.

	spi_retrieve_temp(&high, &low);
			
	send_arr[1] = high;			// SPI temperature sensor readings.
	send_arr[0] = low;
			
	send_arr[4] = 0x55;			// Temperature indicator.
			
	can_send_message(&(send_arr[0]), CAN1_MB0);		//CAN1_MB0 is the data reception MB.
	send_data = 0;
	
	return;
}

void send_coms_packet(void)
{			
	uint8_t i;
	
	for (i = 0; i < 8; i ++)
	{
		send_arr[i] = 0x00;		// Message to be sent back to the OBC.
	}
	
	send_arr[0] = trans_msg[0];	// ASCII character which was received.
	
	can_send_message(&(send_arr[0]), CAN1_MB5);		//CAN1_MB0 is the data reception MB.
	send_coms = 0;
	return;
}

