/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		can_api.h
	*
	*	PURPOSE:	This program contains prototypes and includes required for can_api.c
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
*/
#include "config.h"
#include "can_lib.h"
#include "commands.h"
#include "LED.h"

#define DATA_BUFFER_SIZE 8 // 8 bytes max

/* Global variables to be used for CAN communication */
uint8_t	status, mob_number, send_now, send_hk, send_data, set_sens_h, set_sens_l, set_var;
uint8_t read_response, write_response;
uint8_t receive_arr[8], send_arr[8], read_arr[8], write_arr[8], data_req_arr[8];
uint8_t sensh_arr[8], sensl_arr[8], setv_arr[8];
uint8_t id_array[6];	// Necessary due to the different mailbox IDs for COMS, EPS, PAYL.

/* Global variables used for PUS packet communication */
uint8_t new_tm_msg[8], new_tc_msg[8], tm_sequence_count, new_tm_msgf, current_tm_fullf, tc_packet_readyf;
uint8_t current_tm[143], current_tc[143];

/* Global Variables for EPS		*/
uint16_t pxv_high, pxv_low, pxi_high, pxi_low, pyv_high, pyv_low, pyi_high, pyi_low;
uint16_t battmv_high, battmv_low, battv_high, battv_low, batti_high, batti_low, battemp_high;
uint16_t battemp_low, epstemp_high, epstemp_low, comsv_high, comsv_low, comsi_high, comsi_low;
uint16_t payv_high, payv_low, payi_high, payi_low, obcv_high, obcv_low, obci_high, obci_low;
uint8_t mppta, mpptb;

uint8_t CURRENT_MINUTE;

uint8_t data0[DATA_BUFFER_SIZE];	// Data Buffer for MOb0
uint8_t data1[DATA_BUFFER_SIZE];	// Data Buffer for MOb1
uint8_t data2[DATA_BUFFER_SIZE];	// Data Buffer for MOb2
uint8_t data3[DATA_BUFFER_SIZE];	// Data Buffer for MOb3
uint8_t data4[DATA_BUFFER_SIZE];	// Data Buffer for MOb4
uint8_t data5[DATA_BUFFER_SIZE];	// Data Buffer for MOb5
/*****************************************************/

/* Function Prototypes								 */	
void can_check_general(void);
void can_check_housekeep(void);
void can_send_message(uint8_t* data_array, uint8_t id);
void can_init_mobs(void);
void set_up_msg(uint8_t mailbox);
void clean_up_msg(uint8_t mailbox);
void decode_command(uint8_t* command_array);
/*****************************************************/

