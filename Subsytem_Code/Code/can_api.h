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

#define DATA_BUFFER_SIZE 8 // 8 bytes max

/* Global variables to be used for CAN communication */
uint8_t	status, mob_number, send_now, send_hk, send_data;
uint8_t receive_arr[8], send_arr[8];

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
/*****************************************************/

