/*
Author: Keenan Burnett
***********************************************************************
* FILE NAME: error_handling.h
*
* PURPOSE:
* This file is to be used for the includes, prototypes, and definitions related to error_handling.c
*
* EXTERNAL VARIABLES:
*
* EXTERNAL REFERENCES: global_var.h, can_lib.h
*
* ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None yet.
*
* ASSUMPTIONS, CONSTRAINTS, CONDITIONS: None
*
* NOTES:
*
* REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
*
* DEVELOPMENT HISTORY:
* 12/18/2015		Created.
*
*/

#include "global_var.h"
#include "can_lib.h"

//ERROR IDs as defined in FDIR document

#define SCHED_SPIMEM_R_ERROR			0x01
#define SCHED_SPIMEM_W_ERROR			0x02
#define SCHED_SPIMEM_CHIP_ERROR			0x03
#define SCHED_COMMAND_EXEC_ERROR		0x04
#define SCHED_FIFO_RW_ERROR		        0x05
#define HK_FIFO_RW_ERROR				0x06
#define HK_COLLECT_ERROR				0x07
#define HK_SPIMEM_R_ERROR				0x08
#define HK_SPIMEM_W_ERROR				0x1C
#define TM_FIFO_RW_ERROR				0x08
#define SPIMEM_BUSY_CHIP_ERROR			0x0A
#define SPIMEM_CHIP_ERASE_ERROR			0x0B
#define SPIMEM_LOAD_SECTOR_ERROR		0x0C
#define SPIMEM_UPDATE_SPIBUFFER_ERROR	0x0D
#define SPIMEM_ERASE_SECTOR_ERROR		0x0E
#define SPIMEM_WRITE_SECTOR_ERROR		0x0F
#define SPIMEM_WR_ERROR					0x10
#define SPIMEM_ALL_CHIPS_ERROR			0x11
#define RTC_SPIMEM_R_ERROR				0x12
#define MEM_SPIMEM_CHIPS_ERROR			0x13
#define MEM_SPIMEM_MEM_WASH_ERROR		0x14
#define MEM_OTHER_SPIMEM_ERROR			0x15
#define MEM_FIFO_RW_ERROR				0x16
#define EPS_SSM_GET_SENSOR_DATA_ERROR	0x17
#define EPS_SET_VARIABLE_ERROR			0x18
#define OBC_COMS_TC_TM_ERROR			0x19
#define OBC_TC_PACKET_ERROR				0x1A
#define OBC_FIFO_RW_ERROR				0x1B
#define TC_OK_GO_TIMED_OUT				0x1C		// REPORT ONLY
#define TC_CONSEC_TIMED_OUT				0x1D
#define TM_OK_GO_TIMED_OUT				0x1E		// REPORT ONLY
#define TM_CONSEC_TIMED_OUT				0x1F

uint8_t errorASSERT(uint8_t error, uint8_t* data);
uint8_t errorREPORT(uint8_t error, uint8_t* data);
