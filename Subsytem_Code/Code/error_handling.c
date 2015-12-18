/*
Author: Keenan Burnett
***********************************************************************
* FILE NAME: error_handling.c
*
* PURPOSE:
* This file is to be used to house the functions related to error handling in the SSM code.
*
* FILE REFERENCES: error_handling.h
*
* EXTERNAL VARIABLES:
*
* EXTERNAL REFERENCES: Same a File References.
*
* ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None yet.
*
* ASSUMPTIONS, CONSTRAINTS, CONDITIONS: None
*
* NOTES:
* OBSW = On-Board Software
* GSSW = Groundstation Software
* FDIR = Failure Detection, Isolation & Recovery
*
* REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
*
* DEVELOPMENT HISTORY:
* 12/18/2015		Created.
*
* DESCRIPTION:
* When the SSM come across issues that either need to be resolved immediately, or
* should be made apparent to the FDIR task / ground users, a message needs to be sent to FDIR task
* which is run on the OBC.
*
* This file contains two functions, an "assert" for high severity issues, in which the task
* cannot continue regular operation without the issue being resolved, and an "error-report"
* for low severity issues that should be dealt with but do not prevent that task from continuing
* on with regular operation.
*/
#include "error_handling.h"

// For high-severity errors only.
// Note: This function shall halt the regular operation of this SSM for a maximum of 2 minutes.
// Should the error not be resolved after this time, 0xFF shall be returned.
// 1 is returned if the issue was resolved.
// *data needs to be a uint8_t array of size [3]. 
uint8_t errorASSERT(uint8_t error, uint8_t* data)
{
	uint8_t timeout;
	send_arr[7] = (SELF_ID << 4)|FDIR_TASK_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = SSM_ERROR_ASSERT;
	send_arr[4] = error;
	send_arr[2] = SELF_ID;
	send_arr[2] = *(data + 2);
	send_arr[1] = *(data + 1);
	send_arr[0] = *data;	
	can_send_message(&(send_arr[0]), CAN1_MB7);	
	
	ssm_fdir_signal = 1;
	timeout = 120;
	while(ssm_fdir_signal && timeout--)
	{
		delay_ms(1000);
	}
	if(ssm_fdir_signal)
	{
		ssm_fdir_signal = 0;
		return 0xFF;
	}
	else
		return 1;		// The issue was resolved by the OBC.
}

// For low-severity errors only.
// Note: This functions does not halt regular operation, nor is the error fixed at this time.
// *data needs to be a uint8_t array of size [3]. 
uint8_t errorREPORT(uint8_t error, uint8_t* data)
{
	send_arr[7] = (SELF_ID << 4)|FDIR_TASK_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = SSM_ERROR_REPORT;
	send_arr[4] = error;
	send_arr[2] = SELF_ID;
	send_arr[2] = *(data + 2);
	send_arr[1] = *(data + 1);
	send_arr[0] = *data;
	can_send_message(&(send_arr[0]), CAN1_MB7);	
	return 1;
}
