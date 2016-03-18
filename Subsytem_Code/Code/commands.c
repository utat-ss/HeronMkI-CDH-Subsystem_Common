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
	*					No longer making updates when new functions are added, updates shall be made
	*					if the structure/purpose of this file changes.
	*
*/

#include "commands.h"
#include "global_var.h"

static void store_current_tm(void);
static void send_tc_can_msg(uint8_t packet_count);


/************************************************************************/
/* RUN COMMANDS                                                         */
/*																		*/
/* This function loops through a list of global command flags and		*/
/*	executes the corresponding command function if they are equal to 1.	*/
/************************************************************************/

void run_commands(void)
{
	if (send_now)
		send_response();
	if (send_hk)
		send_housekeeping();
	if (send_data)
	{
		//PIN_toggle(LED2);
		send_sensor_data();
	}
	if (msg_received)
		send_coms_packet();
	if (read_response)
		send_read_response();
	if (write_response)
		send_write_response();
	if (set_sens_h)
		set_sensor_high();
	if (set_sens_l)
		set_sensor_low();
	if (set_varf)
		set_var();
	if (new_tm_msgf)
		receive_tm_msg();
	if (packet_count)
	{
		load_packet_to_current_tc();
		send_pus_packet_tc();
	}
	if (event_readyf)
		send_event();
	if (ask_alive)
		send_ask_alive();
	if (enter_low_powerf)
		enter_low_power();
	if (exit_low_powerf)
		exit_low_power();
	if (enter_take_overf)
		enter_take_over();
	if (exit_take_overf)
		exit_take_over();
	if (pause_operationsf)
		pause_operations();
	if (resume_operationsf)
		resume_operations();
	if (open_valvesf)
		open_valves();
	if (collect_pdf)
		collect_pd();

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
	send_arr[4] = CURRENT_MINUTE;

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
	send_arr[7] = (SELF_ID << 4)|HK_TASK_ID;
	send_arr[6] = MT_HK;	// HK will likely require multiple message in the future.
	send_arr[4] = CURRENT_MINUTE;

#if (SELF_ID == 0)
	if(receiving_tmf)		// Housekeeping takes a while, don't do it while receiving a TM packet.
		return;
	delay_ms(60);
	// Temperature Collection
	send_arr[4] = COMS_TEMP;
	send_arr[1] = 0x00;
	send_arr[0] = 0x44;		// (dummy value for now)
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
#endif

#if (SELF_ID == 1)
	delay_ms(50);
	// EPS Temp Collection
	send_arr[4] = EPS_TEMP;
	send_arr[1] = (uint8_t)(epstemp >> 8);
	send_arr[0] = (uint8_t)epstemp;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);

	// Panel Voltage / Current Collection
	send_arr[4] = PANELX_V;
	send_arr[1] = (uint8_t)(pxv >> 8);
	send_arr[0] = (uint8_t)pxv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PANELX_I;
	send_arr[1] = (uint8_t)(pxi >> 8);
	send_arr[0] = (uint8_t)pxi;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PANELY_V;
	send_arr[1] = (uint8_t)(pyv >> 8);
	send_arr[0] = (uint8_t)pyv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PANELY_I;
	send_arr[1] = (uint8_t)(pyi >> 8);
	send_arr[0] = (uint8_t)pyi;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);

	// Battery Voltage / Current Collection
	send_arr[4] = BATTM_V;
	send_arr[1] = (uint8_t)(battmv >> 8);
	send_arr[0] = (uint8_t)battmv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = BATT_V;
	send_arr[1] = (uint8_t)(battv >> 8);
	send_arr[0] = (uint8_t)battv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = BATTIN_I;
	send_arr[1] = (uint8_t)(battin >> 8);
	send_arr[0] = (uint8_t)battin;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = BATTOUT_I;
	send_arr[1] = (uint8_t)(battout >> 8);
	send_arr[0] = (uint8_t)battout;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);

	// Battery Temperature
	send_arr[4] = BATT_TEMP;
	send_arr[1] = (uint8_t)(epstemp >> 8);
	send_arr[0] = (uint8_t)epstemp;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);

	// Subsystem Voltages and Current Collection
	send_arr[4] = COMS_V;
	send_arr[1] = (uint8_t)(comsv >> 8);
	send_arr[0] = (uint8_t)comsv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = COMS_I;
	send_arr[1] = (uint8_t)(comsi >> 8);
	send_arr[0] = (uint8_t)comsi;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PAY_V;
	send_arr[1] = (uint8_t)(payv >> 8);
	send_arr[0] = (uint8_t)payv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PAY_I;
	send_arr[1] = (uint8_t)(payi >> 8);
	send_arr[0] = (uint8_t)payi;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = OBC_V;
	send_arr[1] = (uint8_t)(obcv >> 8);
	send_arr[0] = (uint8_t)obcv;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = OBC_I;
	send_arr[1] = (uint8_t)(obci >> 8);
	send_arr[0] = (uint8_t)obci;
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
#endif

#if (SELF_ID == 2)
	delay_ms(70);
	// Environmental Sensor Collection
	send_arr[4] = PAY_TEMP0;
	send_arr[1] = 0x00;
	send_arr[0] = 0x66;		// (dummy value for now)
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PAY_HUM;
	send_arr[1] = 0x00;
	send_arr[0] = 0x77;		// (dummy value for now)
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PAY_PRESS;
	send_arr[1] = 0x00;
	send_arr[0] = 0x88;		// (dummy value for now)
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
	send_arr[4] = PAY_ACCEL;
	send_arr[1] = 0x00;
	send_arr[0] = 0x99;		// (dummy value for now)
	can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
	delay_ms(100);
#endif

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
	sensor_name = data_req_arr[4];
	req_by = data_req_arr[7] >> 4;
	send_arr[3] = 0;
	send_arr[2] = 0;
	send_arr[1] = 0;
	send_arr[0] = 0;
	
	if(sensor_name == EPS_TEMP)
	{
		spi_retrieve_temp(&high, &low);
		send_arr[1] = high;			// SPI temperature sensor readings.
		send_arr[0] = low;
	}
	
	if(sensor_name == PANELX_V)
	{
		send_arr[0] = pxv;
	}
	if(sensor_name == PANELX_I)
	{
		send_arr[0] = pxi;
	}
	if(sensor_name == PANELY_V)
	{
		send_arr[0] = pyv;
	}
	if(sensor_name == PANELY_I)
	{
		send_arr[0] = pyi;
	}
	if(sensor_name == BATTM_V)
	{
		send_arr[0] = 0x01;
	}
	if(sensor_name == BATT_V)
	{
		send_arr[0] = 0x02;
	}
	if(sensor_name == BATTIN_I)
	{
		send_arr[0] = 0x03;
	}
	if(sensor_name == BATT_TEMP)
	{
		send_arr[0] = 0x04;
	}
	if(sensor_name == COMS_V)
	{
		send_arr[0] = 0x05;
	}
	if(sensor_name == COMS_I)
	{
		send_arr[0] = 0x06;
	}
	if(sensor_name == PAY_V)
	{
		send_arr[0] = 0x07;
	}
	if(sensor_name == PAY_I)
	{
		send_arr[0] = 0x08;
	}
	if(sensor_name == OBC_V)
	{
		send_arr[0] = 0x09;
	}
	if(sensor_name == OBC_I)
	{
		send_arr[0] = 0x0A;
	}
	if(sensor_name == SHUNT_DPOT)
	{
		send_arr[0] = 0x0B;
	}
	if(sensor_name == COMS_TEMP)
	{
		send_arr[0] = 0x0C;
	}
	if(sensor_name == OBC_TEMP)
	{
		send_arr[0] = 0x0D;
	}
	if(sensor_name == PAY_TEMP0)
	{
		send_arr[0] = 0x0E;
	}
	if(sensor_name == PAY_TEMP1)
	{
		send_arr[0] = 0x0F;
	}
	if(sensor_name == PAY_TEMP2)
	{
		send_arr[0] = 0x10;
	}
	if(sensor_name == PAY_TEMP3)
	{
		send_arr[0] = 0x11;
	}
	if(sensor_name == PAY_TEMP4)
	{
		send_arr[0] = 0x12;
	}
	if(sensor_name == PAY_HUM)
	{
		send_arr[0] = 0x13;
	}
	if(sensor_name == PAY_PRESS)
	{
		send_arr[0] = 0x13;
	}
	if(sensor_name == PAY_ACCEL)
	{
		send_arr[0] = 0x13;
	}

	send_arr[7] = (SELF_ID << 4)|req_by;
	send_arr[6] = MT_DATA;
	send_arr[5] = sensor_name;
	send_arr[4] = CURRENT_MINUTE;			
			
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
	send_arr[4] = CURRENT_MINUTE;
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
	send_arr[4] = CURRENT_MINUTE;
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
	send_arr[4] = CURRENT_MINUTE;
	send_arr[3] = passkey;
	send_arr[0] = ret_val;
	
	can_send_message(&(send_arr[0]), CAN1_MB7);
	write_response = 0;
	return;	
}


void set_sensor_high(void)
{
	uint8_t sensor_name, req_by;
	uint16_t high = 0;
	sensor_name = sensh_arr[3];
	req_by = sensh_arr[7] >> 4;
	
	if(sensor_name == EPS_TEMP)
	{
		epstemp_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		epstemp_high |= (high << 8);
	}
	
	if(sensor_name == PANELX_V)
	{
		pxv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		pxv_high |= (high << 8);		
	}
	
	if(sensor_name == PANELX_I)
	{
		pxi_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		pxi_high |= (high << 8);
	}
	if(sensor_name == PANELY_V)
	{
		pyv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		pyv_high |= (high << 8);
	}
	if(sensor_name == PANELY_I)
	{
		pyi_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		pyi_high |= (high << 8);
	}
	if(sensor_name == BATTM_V)
	{
		battmv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		battmv_high |= (high << 8);
	}
	if(sensor_name == BATT_V)
	{
		battv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		battv_high |= (high << 8);
	}
	if(sensor_name == BATTIN_I)
	{
		pxv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		pxv_high |= (high << 8);
	}
	if(sensor_name == BATTOUT_I)
	{
		pxv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		pxv_high |= (high << 8);
	}
	if(sensor_name == COMS_V)
	{
		comsv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		comsv_high |= (high << 8);
	}
	if(sensor_name == COMS_I)
	{
		comsi_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		comsi_high |= (high << 8);
	}
	if(sensor_name == PAY_V)
	{
		payv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		payv_high |= (high << 8);
	}
	if(sensor_name == PAY_I)
	{
		payi_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		payi_high |= (high << 8);
	}
	if(sensor_name == OBC_V)
	{
		obcv_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		obcv_high |= (high << 8);
	}
	if(sensor_name == OBC_I)
	{
		obci_high = sensh_arr[0];
		high = (uint16_t)sensh_arr[1];
		obci_high |= (high << 8);
	}
	
	set_sens_h = 0;
	return;
}

void set_sensor_low(void)
{
	uint8_t sensor_name, req_by;
	uint16_t low = 0;
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
	if(sensor_name == BATTIN_I)
	{
		battin_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		battin_low |= (low << 8);
	}
	if(sensor_name == BATTOUT_I)
	{
		battout_low = sensl_arr[0];
		low = (uint16_t)sensl_arr[1];
		battout_low |= (low << 8);
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
	
	set_sens_l = 0;
	return;
}

void set_var(void)
{
	uint8_t var_name, incom_val;
	var_name = setv_arr[3];
	incom_val  =setv_arr[0];
	
	switch(var_name)
	{
		case MPPTX:
			mpptx = incom_val;
		case MPPTY:
			mppty = incom_val;
		case BALANCE_H:
			balance_h = incom_val;
		case BALANCE_L:
			balance_l = incom_val;
		case SSM_CTT:
			ssm_consec_trans_timeout = incom_val;
		case SSM_OGT:
			ssm_ok_go_timeout = (uint32_t)incom_val;
		case COMS_FDIR_SIGNAL:
			if(SELF_ID == 0)
				ssm_fdir_signal = incom_val;
		case EPS_FDIR_SIGNAL:
			if(SELF_ID == 1)
				ssm_fdir_signal = incom_val;		
		case PAY_FDIR_SIGNAL:
			if(SELF_ID == 2)
				ssm_fdir_signal = incom_val;
		case BATT_HEAT:
				batt_heater_control = incom_val;
		default:
			set_varf = 0;
			break;
	}
	set_varf = 0;
	return;
}

// Helper
static void send_tm_transaction_response(uint8_t req_by, uint8_t code)
{			
	send_arr[7] = (SELF_ID << 4)|req_by;
	send_arr[6] = MT_COM;
	send_arr[5] = TM_TRANSACTION_RESP;
	send_arr[4] = CURRENT_MINUTE;
	send_arr[3] = 0;
	send_arr[2] = 0;
	send_arr[1] = 0;
	send_arr[0] = code;
	can_send_message(&(send_arr[0]), CAN1_MB2);
	return;
}

//Helper
static void clear_current_tm(void)
{
	uint8_t i;
	for(i = 0; i < PACKET_LENGTH; i++)
	{
		current_tm[i] = 0;
	}
	return;
}

void receive_tm_msg(void)
{
	uint8_t req_by, obc_seq_count;
	req_by = new_tm_msg[7] >> 4;
	obc_seq_count = new_tm_msg[4];
	new_tm_msgf = 0;
	
	if(obc_seq_count > (tm_sequence_count + 1))
	{
		send_tm_transaction_response(req_by, 0xFF);		// Let the OBC know that the transaction failed.
		tm_sequence_count = 0;
		receiving_tmf = 0;
		clear_current_tm();
		return;
	}
	if(current_tm_fullf)
	{
		send_tm_transaction_response(req_by, 0xFF);
		tm_sequence_count = 0;
		receiving_tmf = 0;
		return;
	}
	
	if((!obc_seq_count && !tm_sequence_count) || (obc_seq_count == (tm_sequence_count + 1)))
	{
		tm_sequence_count = obc_seq_count;
		receiving_tmf = 1;
		current_tm[(obc_seq_count * 4)]		= new_tm_msg[0];
		current_tm[(obc_seq_count * 4) + 1] = new_tm_msg[1];
		current_tm[(obc_seq_count * 4) + 2] = new_tm_msg[2];
		current_tm[(obc_seq_count * 4) + 3] = new_tm_msg[3];
		if(obc_seq_count == PACKET_LENGTH / 4 - 1)
		{
			PIN_toggle(LED2);
			tm_sequence_count = 0;									// Reset tm_sequence_count, transmission has completed.
			receiving_tmf = 0;
			current_tm_fullf = 1;									// TM buffer now full, ready to downlink to ground.
			store_current_tm();										// Put current_tm[] into tm_to_downlink[]
			send_tm_transaction_response(req_by, obc_seq_count);	// Let the OBC know that the transaction succeeded.
		}
		return;
	}
	else
	{
		send_tm_transaction_response(req_by, 0xFF);
		tm_sequence_count = 0;
		receiving_tmf = 0;
		clear_current_tm();
		return;
	}
}


// Lets the OBC know that we have a TC packet ready.
void alert_obc_tcp_ready(void)
{
	send_arr[7] = (SELF_ID << 4)|OBC_PACKET_ROUTER_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = TC_PACKET_READY;
	send_arr[4] = CURRENT_MINUTE;
	send_arr[3] = 0;
	send_arr[2] = 0;
	send_arr[1] = 0;
	send_arr[0] = 0;
	can_send_message(&(send_arr[0]), CAN1_MB2);
	return;
}

// This function is necessary so that we can simply trash current_tm if a new transaction fails.
static void store_current_tm(void)
{
	uint8_t i;
	for (i = 0; i < PACKET_LENGTH; i++)
	{
		tm_to_downlink[i] = current_tm[i];
	}
	return;
}

void send_pus_packet_tc(void)
{
	uint8_t i;
	uint8_t num_transfers = PACKET_LENGTH / 4;
	tc_transfer_completef = 0;
	start_tc_transferf = 0;
	long int tc_transfer_time;
	uint8_t data[4];
	
	if(start_tc_transferf)		// A timeout was triggered before the response was received.
		start_tc_transferf = 0;

	alert_obc_tcp_ready();
	tc_transfer_time = millis();
	
	while(!start_tc_transferf) 
	{
		if(millis() - tc_transfer_time > ssm_ok_go_timeout)	// Timeout triggered.
		{
			//PIN_toggle(LED2);
			return;		
		}
		alert_obc_tcp_ready();
		delay_ms(100);
		can_check_general();
		wdt_reset();
	}
	
	/* A TC transaction has now begun	*/	//start_tc_transferf = 1 here.	
	start_tc_transferf = 0;
	//PIN_toggle(LED2);
	for(i = 0; i < num_transfers; i++)
	{
		if(tc_transfer_completef == 0xFF)
		{
			//PIN_toggle(LED2);			
			return;
		}
		//PIN_toggle(LED3);
		send_arr[0] = current_tc[(i * 4)];
		send_arr[1] = current_tc[(i * 4) + 1];
		send_arr[2] = current_tc[(i * 4) + 2];
		send_arr[3] = current_tc[(i * 4) + 3];
		tc_transfer_completef = 0;
		send_tc_can_msg(i);							// Send a TC message to the OBC.
		delay_ms(10);								// Give the OBC 100ms to process that CAN message.
		can_check_general();
		wdt_reset();
	}

	tc_transfer_time = millis();
	while(tc_transfer_completef != (PACKET_LENGTH / 4 - 1))		// Delay for ~10 ms for the OBC to send final transaction response.
	{
		if(millis() - tc_transfer_time > 1000)	// Timeout triggered.
		{
			//PIN_toggle(LED2);
			return;
		}
		can_check_general();
		delay_ms(100);
		wdt_reset();
	}
	
	if(tc_transfer_completef != (PACKET_LENGTH / 4 - 1))
	{
		tc_transfer_completef = 0;
		return;
	}
	else
	{
		PIN_toggle(LED2);				// Transaction Complete!
		tc_transfer_completef = 0;
		tc_packet_readyf = 0;
		return;
	}
}

// It is assumed that send_arr[3-0] have already been filled before executing this function.
static void send_tc_can_msg(uint8_t packet_count)
{
	send_arr[7] = (SELF_ID << 4)|OBC_PACKET_ROUTER_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = SEND_TC;
	send_arr[4] = packet_count;
	can_send_message(&(send_arr[0]), CAN1_MB2);
	return;
}

// event_readyf should be set when an event to report on has been placed in event_arr[]
void send_event(void)
{
	send_arr[7] = (SELF_ID << 4)|OBC_PACKET_ROUTER_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = SEND_EVENT;
	send_arr[4] = CURRENT_MINUTE;
	send_arr[3] = event_arr[3];		// 1=Normal, 2=low-sev error, 3=med-sev, 4=high-sev
	send_arr[2] = event_arr[2];		// ReportID
	send_arr[1] = event_arr[1];
	send_arr[0] = event_arr[0];
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;
}

void send_ask_alive(void)
{
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = ASK_OBC_ALIVE;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	ask_alive = 0;
	return;
}

void enter_low_power(void)
{
	// Sam: Fill this in with what needs to be done for low power mode.
	enter_low_powerf = 0;
	LOW_POWER_MODE = 1;
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = LOW_POWER_MODE_ENTERED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;
}

void exit_low_power(void)
{	
	// Sam: Fill this in with what needs to be done to exit low power mode.
	LOW_POWER_MODE = 0;
	exit_low_powerf = 0;
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = LOW_POWER_MODE_EXITED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;	
}

void enter_take_over(void)
{
	TAKEOVER = 1;
	enter_take_overf = 0;
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = COMS_TAKEOVER_ENTERED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;
}

void exit_take_over(void)
{
	TAKEOVER = 0;
	exit_take_overf = 0;
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = COMS_TAKEOVER_EXITED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;
}

void pause_operations(void)
{
	PAUSE = 1;
	pause_operationsf = 0;
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = OPERATIONS_PAUSED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;	
}

void resume_operations(void)
{
	PAUSE = 0;
	resume_operationsf = 0;
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = OPERATIONS_RESUMED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;	
}

void open_valves(void)
{
	open_valvesf = 0;
	// Open valves here.
	return;
}

void collect_pd(void)
{
	collect_pdf = 0;
	
	// Implement the collection of photodiode data here.
	
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = PD_COLLECTED;
	send_arr[4] = CURRENT_MINUTE;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;
}
