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
	*	08/02/2015		I am changing the way we receive messages though can_check_general()
	*					due to the changes we made as to the structure of our CAN messages.
	*					As of now, the upper 4 bytes of a CAN message correspond to FROM-WHO,
	*					BIG-TYPE, SMALL-TYPE, and TIME respectively. Due to this, can_check_general()
	*					will now be implemented with a case statement and several subsidiary
	*					functions will be added to help accomplish this. In addition, we are
	*					modifying the existing mailbox structure such that:
	*					MB0: Receives data requests.
	*					MB1: Receives commands.
	*					MB2: Receives housekeeping requests.
	*					MB3: Receives Time Checks
	*					MB4,5: Used for sending messages.
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
	uint8_t mb = 0;
	
	for(mb = 0; mb < 4; mb ++)
	{
		set_up_msg(mb);		// Sets up the message object.
		
		if(can_get_status(&message, mob_number) != CAN_STATUS_NOT_COMPLETED) // wait for a message to come in.
		{
			if(message.status == MOB_RX_COMPLETED)
			{
				for (i = 0; i < 8; i ++)		// Transfer the message to the receive array.
				{
					receive_arr[i] = *(message.pt_data + i);
				}
				
				switch(receive_arr[7])	// FROM WHO
				{
					case OBC_ID :
						switch(receive_arr[6]) // BIG TYPE
						{
							case MT_COM :
								decode_command(receive_arr[5]); // SMALL TYPE
							case MT_HK :
								break;
							case MT_DATA :
								break;
							case MT_TC :
								break;
							default:
								break;
						}
					case EPS_ID :
						break;
					case COMS_ID :
						break;
					case PAYL_ID :
						break;
					default:
						break;
				}
				for (i = 0; i < 8; i ++)
				{
					receive_arr[i] = 0;			// Reset the message array to zero after each message.
				}
			}
			
			clean_up_msg(mb);					// Clean up the message object.
		}
	}
	
	can_check_housekeep();
	
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
	uint8_t mb = 5;
	
	set_up_msg(mb);		// Sets up the message object.
		
	if(can_get_status(&message, mob_number) != CAN_STATUS_NOT_COMPLETED) // wait for a message to come in.
	{
		if(message.status == MOB_RX_COMPLETED)
		{
			for (i = 0; i < 8; i ++)		// Transfer the message to the receive array.
			{
				receive_arr[i] = *(message.pt_data + i);
			}
				
			switch(receive_arr[7])	// FROM WHO
			{
				case OBC_ID :
				switch(receive_arr[6]) // BIG TYPE
				{
					case MT_COM :
					decode_command(receive_arr[5]); // SMALL TYPE
					case MT_HK :
						break;
					case MT_DATA :
						break;
					case MT_TC :
						break;
					default:
					break;
				}
				case EPS_ID :
					break;
				case COMS_ID :
					break;
				case PAYL_ID :
					break;
				default:
					break;
			}
			for (i = 0; i < 8; i ++)
			{
				receive_arr[i] = 0;			// Reset the message array to zero after each message.
			}
		}
			
		clean_up_msg(mb);					// Clean up the message object.
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

void decode_command(uint8_t command)
{
	switch(command)
	{
		case REQ_RESPONSE :
			LED_toggle(LED3);	//Toggle LED3 when a response is requested.
			delay_ms(100);
			LED_toggle(LED3);
			delay_ms(100);
			send_now = 1;
			return;
		case REQ_DATA :
			LED_toggle(LED7);	//Toggle LED7 when data is requested.
			delay_ms(100);
			LED_toggle(LED7);
			delay_ms(100);
			send_data = 1;
			return;
		case REQ_HK :
			LED_toggle(LED6);	//Toggle LED6 when housekeeping was requested.
			delay_ms(100);
			LED_toggle(LED6);
			delay_ms(100);
			send_hk = 1;
			return;
		case SMALLTYPE_DEFAULT :
			return;
		default:
			return;
	}
}

void set_up_msg(uint8_t mailbox)
{
	message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
	message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
	message.dlc = 8;			 // Max length of a CAN message.
	mob_number = mailbox;
	
	if(mailbox == 0)
	{
		message.pt_data = &data0[0]; // point message object to first element of data buffer
		message.id.std = SUB0_ID0;  // populate ID field with ID Tag
		return;
	}
	if(mailbox == 1)
	{
		message.pt_data = &data1[0];
		message.id.std = SUB0_ID1;
		return;
	}
	if(mailbox == 2)
	{
		message.pt_data = &data2[0];
		message.id.std = SUB0_ID2;
		return;
	}
	if(mailbox == 3)
	{
		message.pt_data = &data3[0];
		message.id.std = SUB0_ID3;
		return;
	}
	if(mailbox == 5)
	{
		message.pt_data = &data5[0];
		message.id.std = SUB0_ID5;
		return;
	}	
	return;
}

void clean_up_msg(uint8_t mailbox)
{

	if(mailbox == 0)
	{
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = mailbox;
		message.pt_data = &data0[0]; // point message object to first element of data buffer
		message.id.std = SUB0_ID0;  // populate ID field with ID Tag
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
		return;
	}
	if(mailbox == 1)
	{
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = mailbox;
		message.pt_data = &data1[0];
		message.id.std = SUB0_ID1;
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED);
		return;
	}
	if(mailbox == 2)
	{
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = mailbox;
		message.pt_data = &data2[0];
		message.id.std = SUB0_ID2;
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED);
		return;
	}
	if(mailbox == 3)
	{
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = mailbox;
		message.pt_data = &data3[0];
		message.id.std = SUB0_ID3;
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED);
		return;
	}
	if(mailbox == 5)
	{
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = mailbox;
		message.pt_data = &data5[0];
		message.id.std = SUB0_ID5;
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED);
		return;
		
	}
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
	uint8_t i;

	/* INITIALIZE MOB0 */		 // Data reception mailbox.
	message.pt_data = &data0[0]; // point message object to first element of data buffer
	message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
	message.id.std = SUB0_ID0;  // populate ID field with ID Tag
	message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
	message.dlc = 8;			 // Max length of a CAN message.
	mob_number = 0;
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure

	/* INITIALIZE MOB1 */		 // Command reception mailbox.
	message.pt_data = &data1[0];
	message.ctrl.ide = 0;
	message.id.std = SUB0_ID1;
	message.cmd = CMD_RX_DATA;
	message.dlc = 8;
	mob_number = 1;
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
	
	/* INITIALIZE MOB2 */		 // Housekeeping request mailbox.
	message.pt_data = &data2[0];
	message.ctrl.ide = 0;
	message.id.std = SUB0_ID2;
	message.cmd = CMD_RX_DATA;
	message.dlc = 8;
	mob_number = 2;
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
	
	/* INITIALIZE MOB3 */		 // Time-check mailbox.
	message.pt_data = &data3[0];
	message.ctrl.ide = 0;
	message.id.std = SUB0_ID3;
	message.cmd = CMD_RX_DATA;
	message.dlc = 8;
	mob_number = 3;
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
	
	/* INITIALIZE MOB5 */
	
	message.pt_data = &data5[0];	// point message object to first element of data buffer
	message.ctrl.ide = 0;			// standard CAN frame type (2.0A)
	message.id.std = SUB0_ID5;		// populate ID field with ID Tag
	message.cmd = CMD_RX_DATA;		// assign this as a producer message object (Housekeeping MOB).
	message.dlc = 8;				// Max length of a CAN message.
	mob_number = 5;
	
	while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOB to configure
	
	for (i = 0; i < 8; i ++)
	{
		receive_arr[i] = 0;			// Reset the message array to zero after each message.
	}
	
	send_now = 0;
	send_hk = 0;
	send_data = 0;
	
	return;
}

