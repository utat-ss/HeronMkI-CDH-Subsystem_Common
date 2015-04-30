/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		can_api.c
	*
	*	PURPOSE:	This program contains API functions related to CAN communication.
	*
	*	FILE REFERENCES:
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
	*	03/07/2015		Created.
	*
	*	03/28/2015		I edited can_send_message so that it now takes in an id as a
	*					parameter. This is due to the fact that we are now actually
	*					using different mailboxes in the OBC for different functions
	*					now.
	*
*/

/************************************************************************/
/*	CHECK FOR A GENERAL CAN MESSAGE                                     */
/*																		*/
/*	This function checks to see if a general can message has been		*/
/*	received in MOB0. If the message matches to one currently in		*/
/*	our message library, then we set a flag which will indicate that an	*/
/*	action needs to be performed some time in the current program loop.	*/
/*																		*/
/*	These flags include send_now for commands and send_data for data.	*/
/************************************************************************/

#include "can_api.h"

void can_check_general(void)
{
	uint8_t i = 0;
	
	message.pt_data = &data0[0]; // point message object to first element of data buffer
	message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
	message.id.std = SUB0_ID0;  // populate ID field with ID Tag
	message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
	message.dlc = 8;			 // Max length of a CAN message.
	mob_number = 0;
	
	
	if(can_get_status(&message, mob_number) != CAN_STATUS_NOT_COMPLETED) // wait for a message to come in.
	{
		if(message.status == MOB_RX_COMPLETED)
		{
			for (i = 0; i < 8; i ++)
			{
				receive_arr[i] = *(message.pt_data + i);
			}
			
			if ((receive_arr[0] == 0xFF) && (receive_arr[1] == 0xFF) && (receive_arr[2] == 0xFF) && (receive_arr[3] == 0xFF)
			&& (receive_arr[4] == 0xFF) && (receive_arr[5] == 0xFF) && (receive_arr[6] == 0xFF) && (receive_arr[7] == 0xFF))
			{
				LED_toggle(LED3);	//Toggle LED3 when the appropriate message is received.
				delay_ms(100);
				LED_toggle(LED3);
				delay_ms(100);
				send_now = 1;
			}
			
			if ((receive_arr[0] == 0x55) && (receive_arr[1] == 0x55) && (receive_arr[2] == 0x55) && (receive_arr[3] == 0x55)
			&& (receive_arr[4] == 0x55) && (receive_arr[5] == 0x55) && (receive_arr[6] == 0x55) && (receive_arr[7] == 0x55))
			{
				LED_toggle(LED7);	//Toggle LED7 when the appropriate message is received.
				delay_ms(100);
				LED_toggle(LED7);
				delay_ms(100);
				send_data = 1;
			}
			
			for (i = 0; i < 8; i ++)
			{
				receive_arr[i] = 0;			// Reset the message array to zero after each message.
			}
		}
		message.pt_data = &data0[0]; // point message object to first element of data buffer
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.id.std = SUB0_ID0;  // populate ID field with ID Tag
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = 0;
		
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
	}
	return;
}

/************************************************************************/
/*	CHECK FOR A HOUSEKEEPING REQUEST		                            */
/*																		*/
/*	This function checks to see if housekeeping has been requested, if  */
/*	it has, then we set the flag send_hk to 1.							*/
/************************************************************************/
void can_check_housekeep(void)
{
	uint8_t i = 0;
	
	message.pt_data = &data5[0]; // point message object to first element of data buffer
	message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
	message.id.std = SUB0_ID5;  // populate ID field with ID Tag
	message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
	message.dlc = 8;			 // Max length of a CAN message.
	mob_number = 5;
	
	if(can_get_status(&message, mob_number) != CAN_STATUS_NOT_COMPLETED) // wait for a housekeeping request to come in.
	{
		if(message.status == MOB_RX_COMPLETED)
		{
			for (i = 0; i < 8; i ++)
			{
				receive_arr[i] = *(message.pt_data + i);
			}
			
			if ((receive_arr[0] == 0x0F) && (receive_arr[1] == 0x0F) && (receive_arr[2] == 0x0F) && (receive_arr[3] == 0x0F)
			&& (receive_arr[4] == 0x0F) && (receive_arr[5] == 0x0F) && (receive_arr[6] == 0x0F) && (receive_arr[7] == 0x0F))
			{
				LED_toggle(LED6);	//Toggle LED5 when housekeeping was requested.
				delay_ms(100);
				LED_toggle(LED6);
				delay_ms(100);
				send_hk = 1;
			}
			for (i = 0; i < 8; i ++)
			{
				receive_arr[i] = 0;			// Reset the message array to zero after each message.
			}
		}
		message.pt_data = &data5[0]; // point message object to first element of data buffer
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.id.std = SUB0_ID5;  // populate ID field with ID Tag
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = 5;
		
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
	}
	return;
}


/************************************************************************/
/*		SEND A CAN MESSAGE	                                            */
/*																		*/
/*		This function takes in an array which is the message which is	*/
/*		meant to be sent and then executes the required operations		*/
/*		to send the can message. In the future, the node ID, mob_number	*/
/*		and data array being used should be parameters to this function.*/
/************************************************************************/

void can_send_message(uint8_t* data_array, uint8_t id)
{
	uint8_t i = 0;

	message.pt_data = &data4[0]; // point message object to first element of data buffer
	message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
	message.id.std = id;  // populate ID field with ID Tag
	message.cmd = CMD_TX_DATA;   // assign this as a transmitting message object.
	message.dlc = 8;			 // Max length of a CAN message.
	mob_number = 4;
	
	for (i = 0; i < 8; i ++)
	{
		data4[i] = *(data_array + i) ;		// Message to be sent back to the OBC.
	}
	
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb4 to configure

	while(can_get_status(&message, mob_number) == CAN_STATUS_NOT_COMPLETED); // wait for a message to send or fail.

	return;
}

/************************************************************************/
/*		INITIALIZE CAN MESSAGE OBJECTS                                  */
/*																		*/
/*		This function initializes our can message objects with their ID	*/
/*		and sets whether or not they are in transmit mode or receive.	*/
/************************************************************************/

void can_init_mobs(void)
{
	/* INITIALIZE MOB0 */

	message.pt_data = &data0[0]; // point message object to first element of data buffer
	message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
	message.id.std = SUB0_ID0;  // populate ID field with ID Tag
	message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
	message.dlc = 8;			 // Max length of a CAN message.
	mob_number = 0;
	
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
	
	/* INITIALIZE MOB5 */
	
	message.pt_data = &data5[0];	// point message object to first element of data buffer
	message.ctrl.ide = 0;			// standard CAN frame type (2.0A)
	message.id.std = SUB0_ID5;		// populate ID field with ID Tag
	message.cmd = CMD_RX_DATA;		// assign this as a producer message object (Housekeeping MOB).
	message.dlc = 8;				// Max length of a CAN message.
	mob_number = 5;
	
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOB to configure
}
