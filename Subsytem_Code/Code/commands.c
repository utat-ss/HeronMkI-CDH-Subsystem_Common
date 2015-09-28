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
	*	08/07/2015		Modified the send... functions in order to comply with the new
	*					CAN structure that I have been working on.
	*	08/08/2015		I modified the send functions again in accordance with the
	*					change that I have made to the CAN structure, this being that
	*					the first byte of a CAN message now contains both the sender's
	*					ID and the destination ID.
	*
	*	08/09/2015		Functions send_read_response(), and send_write_response() have been added.
	*
*/

#include "commands.h"

/************************************************************************/
/* RUN COMMANDS                                                         */
/*																		*/
/* This function loops through a list of global command flags and		*/
/*	executes the corresponding command function if they are equal to 1.	*/
/************************************************************************/

void run_commands(void)
{
	if (send_now == 1)
		send_response();
	if (send_hk == 1)
		send_housekeeping();
	if (send_data == 1)
		send_sensor_data();
	if (msg_received == 1)
		send_coms_packet();
	if (read_response == 1)
		send_read_response();
	if (write_response == 1)
		send_write_response();
	if (set_sens_h == 1)
		set_sensor_highf();
	if (set_sens_l == 1)
		set_sensor_lowf();
	if (set_var == 1)
		set_varf();
	

	return;	
}

/************************************************************************/
/* SEND RESPONSE                                                        */
/*																		*/
/* Thia function sends a generic response to the generic "REQ_RESPONSE	*/
/* which was issued by the OBC.											*/
/************************************************************************/
void send_response(void)
{
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = RESPONSE;

	can_send_message(&(send_arr[0]), CAN1_MB7);		//CAN1_MB7 is the command reception MB.
	send_now = 0;
	return;
}

/************************************************************************/
/* SEND HOUSEKEEPING                                                    */
/*																		*/
/* This function is intended to be used to send housekeeping to the OBC.*/
/* It is likely that more than 4 bytes will be required to send the HK	*/
/* from a particular subsystem and hence this function will send a		*/
/* series of CAN messages to the OBC each one with a different smalltype*/
/************************************************************************/

void send_housekeeping(void)
{	
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_HK;	// HK will likely require multiple message in the future.

	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	send_hk = 0;
	return;
}

/************************************************************************/
/* SEND SENSOR DATA                                                     */
/*																		*/
/* This is an example of a generic sensor data transmission function	*/
/* that we will likely have in the future. An SSM would only execute	*/
/* this command upon request from the OBC.								*/
/************************************************************************/

void send_sensor_data(void)
{
	uint8_t high, low, sensor_name, req_by;			
	sensor_name = data_req_arr[3];
	req_by = data_req_arr[7] >> 4;
	
	if(sensor_name == EPS_TEMP)
	{
		spi_retrieve_temp(&high, &low);
		send_arr[1] = high;			// SPI temperature sensor readings.
		send_arr[0] = low;
	}
	
	//if(sensor_name == PANELX_V)
	//{
	//}
	//if(sensor_name == PANELX_I)
	//{
	//}
	//if(sensor_name == PANELY_V)
	//{
	//}
	//if(sensor_name == PANELY_I)
	//{
	//}
	//if(sensor_name == BATTM_V)
	//{
	//}
	//if(sensor_name == BATT_V)
	//{
	//}
	//if(sensor_name == BATT_I)
	//{
	//}
	//if(sensor_name == BATT_TEMP)
	//{
	//}
	//if(sensor_name == COMS_V)
	//{
	//}
	//if(sensor_name == COMS_I)
	//{
	//}
	//if(sensor_name == PAY_V)
	//{
	//}
	//if(sensor_name == PAY_I)
	//{
	//}
	//if(sensor_name == OBC_V)
	//{
	//}
	//if(sensor_name == OBC_I)
	//{
	//}

	send_arr[7] = (SELF_ID << 4)|req_by;
	send_arr[6] = MT_DATA;
	send_arr[5] = sensor_name;				
			
	can_send_message(&(send_arr[0]), CAN1_MB0);		//CAN1_MB0 is the data reception MB.
	send_data = 0;
	
	return;
}

/************************************************************************/
/* SEND COMS PACKET                                                     */
/*																		*/
/* For the time being, this function simply sends a single ASCII		*/
/* character to the OBC when a packet is received from the transceiver. */
/* In the future, this function will be able to send entire packets		*/
/* to the OBC as two CAN messages (2 x 4 bytes).						*/
/************************************************************************/
void send_coms_packet(void)
{			
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_DATA;
	send_arr[5] = COMS_PACKET;
	send_arr[0] = trans_msg[0];	// ASCII character which was received.
	
	can_send_message(&(send_arr[0]), CAN1_MB0);		//CAN1_MB0 is the data reception MB.
	msg_received = 0;
	return;
}
/************************************************************************/
/* SEND READ RESPONSE                                                   */
/*																		*/
/* Whent the OBC requests to read an address in the SSM's memory, this	*/
/* function carries out that task with the use of global array read_arr	*/
/* read_arr stores the most read request from the OBC.					*/
/************************************************************************/

void send_read_response(void)
{
	uint8_t read_val, passkey, req_by;
	uint8_t* read_ptr;
	
	passkey = read_arr[3];
	read_ptr = read_arr[0];
	req_by = read_arr[7];	// Used to coordinating with tasks on the OBC.
	
	/*	Execute the read	*/
	read_val = *read_ptr;
	
	send_arr[7] = (SELF_ID << 4)|req_by;
	send_arr[6] = MT_COM;
	send_arr[5] = ACK_READ;
	send_arr[3] = passkey;
	send_arr[0] = read_val;
	
	can_send_message(&(send_arr[0]), CAN1_MB7);
	read_response = 0;
	return;
}

/************************************************************************/
/* SEND WRITE RESPONSE                                                  */
/*																		*/
/* When the OBC requests to write to an address in the SSM's memory,	*/
/* this function carries out that task with global array write_arr.		*/
/* write_arr stores the most recent write request. -1 is given to the	*/
/* OBC as FAILURE, and 1 is returned as SUCCESS.						*/
/************************************************************************/
void send_write_response(void)
{
	uint8_t passkey, write_data, ret_val, verify, req_by;
	uint8_t* write_ptr;
	
	passkey = write_arr[3];
	write_ptr = write_arr[1];
	write_data = write_arr[0];
	req_by = read_arr[7];	// Used to coordinating with tasks on the OBC.
	
	/*	Execute the Write	*/
	*write_ptr = write_data;
	
	/* Verify the write worked correctly	*/
	verify = *write_ptr;
	if (verify != write_data)
		ret_val = -1;
	else
		ret_val = 1;
	
	send_arr[7] = (SELF_ID << 4)|req_by;
	send_arr[6] = MT_COM;
	send_arr[5] = ACK_WRITE;
	send_arr[3] = passkey;
	send_arr[0] = ret_val;
	
	can_send_message(&(send_arr[0]), CAN1_MB7);
	write_response = 0;
	return;	
}


void set_sensor_highf(void)
{
	uint8_t low, sensor_name, req_by;
	sensor_name = sensh_arr[3];
	req_by = sensh_arr[7] >> 4;
	
	if(sensor_name == EPS_TEMP)
	{
		epstemp_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		epstemp_high |= (low << 8);
	}
	
	if(sensor_name == PANELX_V)
	{
		pxv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		pxv_high |= (low << 8);		
	}
	
	if(sensor_name == PANELX_I)
	{
		pxi_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		pxi_high |= (low << 8);
	}
	if(sensor_name == PANELY_V)
	{
		pyv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		pyv_high |= (low << 8);
	}
	if(sensor_name == PANELY_I)
	{
		pyi_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		pyi_high |= (low << 8);
	}
	if(sensor_name == BATTM_V)
	{
		battmv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		battmv_high |= (low << 8);
	}
	if(sensor_name == BATT_V)
	{
		battv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		battv_high |= (low << 8);
	}
	if(sensor_name == BATT_I)
	{
		pxv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		pxv_high |= (low << 8);
	}
	if(sensor_name == BATT_TEMP)
	{
		battemp_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		battemp_high |= (low << 8);
	}
	if(sensor_name == COMS_V)
	{
		comsv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		comsv_high |= (low << 8);
	}
	if(sensor_name == COMS_I)
	{
		comsi_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		comsi_high |= (low << 8);
	}
	if(sensor_name == PAY_V)
	{
		payv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		payv_high |= (low << 8);
	}
	if(sensor_name == PAY_I)
	{
		payi_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		payi_high |= (low << 8);
	}
	if(sensor_name == OBC_V)
	{
		obcv_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		obcv_high |= (low << 8);
	}
	if(sensor_name == OBC_I)
	{
		obci_high = sensh_arr[0];
		low = (uint16_t)sensh_arr[1];
		obci_high |= (low << 8);
	}
		
	return;
}

void set_sensor_lowf(void)
{
	uint8_t low, sensor_name, req_by;
	sensor_name = sensl_arr[3];
	req_by = sensl_arr[7] >> 4;
	
	if(sensor_name == EPS_TEMP)
	{
		epstemp_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		epstemp_low |= (low << 8);
	}
	
	if(sensor_name == PANELX_V)
	{
		pxv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		pxv_low |= (low << 8);
	}
	
	if(sensor_name == PANELX_I)
	{
		pxi_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		pxi_low |= (low << 8);
	}
	if(sensor_name == PANELY_V)
	{
		pyv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		pyv_low |= (low << 8);
	}
	if(sensor_name == PANELY_I)
	{
		pyi_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		pyi_low |= (low << 8);
	}
	if(sensor_name == BATTM_V)
	{
		battmv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		battmv_low |= (low << 8);
	}
	if(sensor_name == BATT_V)
	{
		battv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		battv_low |= (low << 8);
	}
	if(sensor_name == BATT_I)
	{
		pxv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		pxv_low |= (low << 8);
	}
	if(sensor_name == BATT_TEMP)
	{
		battemp_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		battemp_low |= (low << 8);
	}
	if(sensor_name == COMS_V)
	{
		comsv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		comsv_low |= (low << 8);
	}
	if(sensor_name == COMS_I)
	{
		comsi_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		comsi_low |= (low << 8);
	}
	if(sensor_name == PAY_V)
	{
		payv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		payv_low |= (low << 8);
	}
	if(sensor_name == PAY_I)
	{
		payi_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		payi_low |= (low << 8);
	}
	if(sensor_name == OBC_V)
	{
		obcv_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		obcv_low |= (low << 8);
	}
	if(sensor_name == OBC_I)
	{
		obci_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		obci_low |= (low << 8);
	}
	
	return;
}

void set_varf(void)
{
	uint8_t low, var_name;
	var_name = setv_arr[3];
	
	if(var_name == MPPTA)
	{
		mppta = setv_arr[0];
	}
	if(var_name == MPPTB)
	{
		mpptb = setv_arr[0];
	}
}