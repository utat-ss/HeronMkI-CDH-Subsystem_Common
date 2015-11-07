/*
	Copyright (c) 2007 Atmel.
	Edited by Keenan Burnett

	***********************************************************************
	*	FILE NAME:		can_lib.h
	*
	*	PURPOSE:		
	*	This is the main program which shall be run on the ATMEGA32M1s to be used on subsystem
	*	microcontrollers.
	*
	*	This file contains the prototypes and macros of the library of functions of:
	*			CAN (Controller Array Network)
	*			AT90CAN128/64/32
	*
	*	This file can be parsed by Doxygen for automatic documentation generation.
	*	This file has been validated with AVRStudio-413528/WinAVR-20070122.
	*
	*	FILE REFERENCES:	can_drv.h
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
	*		The contents of this file were originally from the CAN Software Library
	*		provided by Atmel written for AT90CAN devices. Use of this file is subject
	*		to Atmel's End User License Agreement.
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	01/02/2015		Created.
	*
	*	02/06/2015		Edited the header.
	*
*/


#ifndef _CAN_LIB_H_
#define _CAN_LIB_H_

//_____ I N C L U D E S ________________________________________________________
#include "can_drv.h"

//_____ D E F I N I T I O N S __________________________________________________

/*				MY CAN DEFINES								*/
#define SELF_ID					1 // Current SSM is EPS.

#define PACKET_LENGTH			143	// Length of the PUS packet.

#define COMMAND_OUT					0X01010101
#define COMMAND_IN					0x11111111

#define HK_TRANSMIT					0x12345678
#define CAN_MSG_DUMMY_DATA          0xFFFFFFFF

#define DUMMY_COMMAND				0XFFFFFFFF
#define MSG_ACK						0xABABABAB

#define HK_RETURNED					0XF0F0F0F0
#define HK_REQUEST					0x0F0F0F0F

#define DATA_REQUEST				0x55555555
#define DATA_RETURNED				0x00000055

#define MESSAGE_RETURNED			0X00000000

#define CAN0_MB0				1
#define CAN0_MB1				2
#define CAN0_MB2				3
#define CAN0_MB3				4
#define CAN0_MB4				5
#define CAN0_MB5				6
#define CAN0_MB6				7
#define CAN0_MB7				8

#define CAN1_MB0				10
#define CAN1_MB1				11
#define CAN1_MB2				12
#define CAN1_MB3				13
#define CAN1_MB4				14
#define CAN1_MB5				15
#define CAN1_MB6				16
#define CAN1_MB7				17

/* IDs for COMS/SUB0 mailboxes */
#define SUB0_ID0				20
#define SUB0_ID1				21
#define SUB0_ID2				22
#define SUB0_ID3				23
#define SUB0_ID4				24
#define SUB0_ID5				25

/* IDs for EPS/SUB1 mailboxes */
#define SUB1_ID0				26
#define SUB1_ID1				27
#define SUB1_ID2				28
#define SUB1_ID3				29
#define SUB1_ID4				30
#define SUB1_ID5				31

/* IDs for PAYLOAD/SUB2 mailboxes */
#define SUB2_ID0				32
#define SUB2_ID1				33
#define SUB2_ID2				34
#define SUB2_ID3				35
#define SUB2_ID4				36
#define SUB2_ID5				37

/* MessageType_ID  */
#define MT_DATA					0x00
#define MT_HK					0x01
#define MT_COM					0x02
#define MT_TC					0x03

/* SENDER_ID */
#define COMS_ID					0x00
#define EPS_ID					0x01
#define PAY_ID					0x02
#define OBC_ID					0x03
#define HK_TASK_ID				0x04
#define DATA_TASK_ID			0x05
#define TC_TASK_ID				0x06
#define COMS_TASK_ID			0x07
#define EPS_TASK_ID				0x08
#define PAY_TASK_ID				0x09
#define OBC_PACKET_ROUTER_ID	0x0A
#define SCHEDULING_TASK_ID		0x0B
#define WD_RESET_TASK_ID		0x0D
#define MEMORY_TASK_ID			0x0E
#define TIME_TASK_ID			0x0F
#define HK_GROUND_ID			0x10

/* COMMAND SMALL-TYPE: */
#define REQ_RESPONSE			0x01
#define REQ_DATA				0x02
#define REQ_HK					0x03
#define RESPONSE 				0x04
#define REQ_READ				0x05
#define ACK_READ				0x06
#define REQ_WRITE				0x07
#define ACK_WRITE				0x08
#define SET_SENSOR_HIGH			0x09	// Remember to put SENSOR_NAME into Byte 3.
#define SET_SENSOR_LOW			0x0A
#define SET_VAR					0x0B	// Remember to put VAR_NAME into Byte 3.
#define SET_TIME				0x0C
#define SEND_TM					0x0D
#define SEND_TC					0x0E
#define TM_PACKET_READY			0x0F
#define OK_START_TM_PACKET		0x10
#define TC_PACKET_READY			0x11
#define OK_START_TC_PACKET		0x12
#define TM_TRANSACTION_RESP		0x13
#define TC_TRANSACTION_RESP		0x14

/* Checksum only */
#define SAFE_MODE_VAR			0x09

#define SMALLTYPE_DEFAULT		0x00

/* DATA SMALL-TYPE	   */
#define SPI_TEMP1				0x01
#define COMS_PACKET				0x02
#define BATT_TOP				0x03
#define BATT_BOTTOM				0x04

/* MESSAGE PRIORITIES	*/
#define COMMAND_PRIO			25
#define HK_REQUEST_PRIO			20
#define DATA_PRIO				10
#define DEF_PRIO				10

/* SENSOR NAMES			*/
#define PANELX_V				0x01
#define PANELX_I				0x02
#define PANELY_V				0x03
#define PANELY_I				0x04
#define BATTM_V					0x05
#define BATT_V					0x06
#define BATTIN_I				0x07
#define BATTOUT_I				0x08
#define BATT_TEMP				0x09
#define EPS_TEMP				0x0A
#define COMS_V					0x0B
#define COMS_I					0x0C
#define PAY_V					0x0D
#define PAY_I					0x0E
#define OBC_V					0x0F
#define OBC_I					0x10
#define BATT_I					0x11
#define COMS_TEMP				0x12
#define OBC_TEMP				0x13
#define PAY_TEMP0				0x14
#define PAY_TEMP1				0x15
#define PAY_TEMP2				0x16
#define PAY_TEMP3				0x17
#define PAY_TEMP4				0x18
#define PAY_HUM					0x19
#define PAY_PRESS				0x1A

/* VARIABLE NAMES		*/
#define MPPTA					0xFF
#define MPPTB					0xFE
#define COMS_MODE				0xFD
#define EPS_MODE				0xFC
#define PAY_MODE				0xFB
#define OBC_MODE				0xFA
#define PAY_STATE				0xF9
#define ABS_TIME_D				0xF8
#define ABS_TIME_H				0xF7
#define ABS_TIME_M				0xF6
#define ABS_TIME_S				0xF5
#define SPI_CHIP_1				0xF4
#define SPI_CHIP_2				0xF3
#define SPI_CHIP_3				0xF2

/* CAN frame max data length */
#define MAX_CAN_FRAME_DATA_LEN      8

/*********************************************************************/

// ----------
// @brief This constant is used as return value for "can_cmd" function.
#define CAN_CMD_REFUSED  0xFF
// ----------
// @brief This constant is used as return value for "can_cmd" function.
#define CAN_CMD_ACCEPTED         0x00

// ----------
// @brief This constant is used as return value for "can_get_status" function.
#define CAN_STATUS_COMPLETED     0x00
// ----------
// @brief This constant is used as return value for "can_get_status" function.
#define CAN_STATUS_NOT_COMPLETED 0x01
// ----------
// @brief This constant is used as return value for "can_get_status" function.
#define CAN_STATUS_ERROR         0x02

// ----------
// @brief This enumeration is used to select an action for a specific message
// declared in st_cmd_t structure.
typedef enum {
  CMD_NONE,
  CMD_TX,
  CMD_TX_DATA,
  CMD_TX_REMOTE,
  CMD_RX, 
  CMD_RX_DATA, 
  CMD_RX_REMOTE,
  CMD_RX_MASKED,
  CMD_RX_DATA_MASKED,
  CMD_RX_REMOTE_MASKED, 
  CMD_REPLY,        
  CMD_REPLY_MASKED,
  CMD_ABORT
} can_cmd_t; 

// ----------
// @brief This union defines a CAN identifier and allows to access it in mode
// standard, extended or through a table.
typedef union{
  uint32_t ext;
  uint16_t std;
  uint8_t  tab[4];
} can_id_t; 

// ----------
// @brief This structure defines some specific information as RTR bit and
// IDE bit
typedef struct{
  Bool rtr;
  Bool ide;
} can_ctrl_t;

// ----------
// @brief This structure allows to define a specific action on CAN network.
// 1) handle:  Equivalent to the MOb Number.
// 2) cmd   :  initialize by the application to select the operation.
// 3) id    :  initialize by the application in transmission
//             complete by the library in reception.
// 4) dlc   :  initialize by the application to give the number of data to 
//             transmit complete by the library in reception.
// 5) pt_data: pointer on the table which contains the data to send or
//             received.
// 6) status:  manage by the library.
// 7) ctrl  :  field ide to signal a extended frame .
typedef  struct{
  uint8_t         handle; 
  can_cmd_t  cmd; 
  can_id_t   id;
  uint8_t         dlc;  
  uint8_t*        pt_data; 
  uint8_t         status; 
  can_ctrl_t ctrl;   
} st_cmd_t;


//_____ D E C L A R A T I O N S ________________________________________________
st_cmd_t message; // message object


//------------------------------------------------------------------------------
//  @fn can_init
//!
//! CAN macro initialization. Reset the CAN peripheral, initialize the bit
//! timing, initialize all the registers mapped in SRAM to put MObs in
//! inactive state and enable the CAN macro.
//!
//! @warning The CAN macro will be enable after seen on CAN bus a receceive
//!          level as long as of an inter frame (hardware feature).
//!
//! @param  Mode (for "can_fixed_baudrate" param not used)
//!         ==0: start CAN bit timing evaluation from faster baudrate
//!         ==1: start CAN bit timing evaluation with CANBTx registers
//!              contents
//!
//! @return Baudrate Status
//!         ==0: research of bit timing configuration failed
//!         ==1: baudrate performed 
//!
extern uint8_t can_init(uint8_t mode);

//------------------------------------------------------------------------------
//  @fn can_cmd
//!
//! This function takes a CAN descriptor, analyses the action to do:
//! transmit, receive or abort.
//! This function returns a status (CAN_CMD_ACCEPTED or CAN_CMD_REFUSED) if
//! a MOb for Rx or Tx has been found. If no MOB has been found, the
//! application must be retry at a later date.
//! This function also updates the CAN descriptor status (MOB_PENDING or
//! MOB_NOT_REACHED) if a MOb for Rx or Tx has been found. If aborting
//! is performed, the CAN descriptor status will be set to STATUS_CLEARED.
//!
//! @param  st_cmd_t* - Can_descriptor pointer on CAN descriptor structure
//!         to select the action to do.
//!
//! @return CAN_CMD_ACCEPTED - command is accepted
//!         CAN_CMD_REFUSED  - command is refused
//!
extern uint8_t can_cmd(st_cmd_t* cmd, uint8_t mob_number);
//------------------------------------------------------------------------------
//  @fn can_get_status
//!
//! This function allows to return if the command has been performed or not.
//! In an reception case, all the CAN message is stored in the structure.
//! This function also updates the CAN descriptor status (MOB_TX_COMPLETED,    
//!  MOB_RX_COMPLETED, MOB_RX_COMPLETED_DLCW or MOB_???_ERROR).         
//!
//! @param  st_cmd_t* pointer on CAN descriptor structure.
//!
//! @return CAN_STATUS_COMPLETED     - Rx or Tx is completed
//!         CAN_STATUS_NOT_COMPLETED - Rx or Tx is not completed
//!         CAN_STATUS_ERROR         - Error in configuration or in the
//!                                    CAN communication
//!
extern uint8_t can_get_status (st_cmd_t* cmd, uint8_t mob_number);

//______________________________________________________________________________

#endif // _CAN_LIB_H_

