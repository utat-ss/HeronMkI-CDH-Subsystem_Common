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
#define CAN_MAM_MIDvA_Pos 18
#define CAN_MAM_MIDvA_Msk (0x7ffu << CAN_MAM_MIDvA_Pos) /**< \brief (CAN_MAM) Identifier for standard frame mode */

#define CAN_MAM_MIDvB_Pos 0
#define CAN_MAM_MIDvB_Msk (0x3ffffu << CAN_MAM_MIDvB_Pos) /**< \brief (CAN_MAM) Complementary bits for identifier in extended frame mode */
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
extern uint8_t can_cmd (st_cmd_t *);

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
extern uint8_t can_get_status (st_cmd_t *); 

//______________________________________________________________________________

#endif // _CAN_LIB_H_

