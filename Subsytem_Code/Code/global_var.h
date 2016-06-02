/*
    Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		global_var.h
	*
	*	PURPOSE:		This file contains global variable definitions to be used across all files.
	*	FILE REFERENCES:		None
	*
	*	EXTERNAL VARIABLES:		None
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None yet.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:					All global variables and definitions should be put in this file.
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*
	*	DEVELOPMENT HISTORY:
	*	11/12/2015			Created.
	*
*/
#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <stdint.h>

typedef struct{
	uint8_t data[152];
} packet;


#define DATA_BUFFER_SIZE		8 // 8 bytes max

/*				MY CAN DEFINES								*/
#define SELF_ID					1 // Current SSM is EPS.

#define PUS_COMMUNICATION_ON	0 // Note: If PUS_COMMUNICATION_ON == 1, other SSMs will not be 
								  // programmable from the laptop interface.
								  
#define UART_DISABLE			0 // Note: If UART_DISABLE == 0, other SSMs will not be programmable
								  // from the laptop interface

#define PACKET_LENGTH			152	// Length of the PUS packet.

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
#define CAN1_MB1				10
#define CAN1_MB2				11
#define CAN1_MB3				11
#define CAN1_MB4				11
#define CAN1_MB5				14
#define CAN1_MB6				14
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
#define TIME_TASK_ID			0x06
#define COMS_TASK_ID			0x07
#define EPS_TASK_ID				0x08
#define PAY_TASK_ID				0x09
#define OBC_PACKET_ROUTER_ID	0x0A
#define SCHEDULING_TASK_ID		0x0B
#define FDIR_TASK_ID			0x0C
#define WD_RESET_TASK_ID		0x0D
#define MEMORY_TASK_ID			0x0E
#define HK_GROUND_ID			0x0F
#define TIME_GROUND_ID			0x10
#define MEM_GROUND_ID			0x11
#define GROUND_PACKET_ROUTER_ID 0x13
#define FDIR_GROUND_ID			0x14
#define SCHED_GROUND_ID			0x15

/* COMMAND SMALL-TYPE: */
#define REQ_RESPONSE			0x01
#define REQ_DATA				0x02
#define REQ_HK					0x03
#define RESPONSE 				0x04
#define REQ_READ				0x05
#define ACK_READ				0x06
#define REQ_WRITE				0x07
#define ACK_WRITE				0x08
#define SET_SENSOR_HIGH			0x09
#define SET_SENSOR_LOW			0x0A
#define SET_VAR					0x0B
#define SET_TIME				0x0C
#define SEND_TM					0x0D
#define SEND_TC					0x0E
#define TM_PACKET_READY			0x0F
#define OK_START_TM_PACKET		0x10
#define TC_PACKET_READY			0x11
#define OK_START_TC_PACKET		0x12
#define TM_TRANSACTION_RESP		0x13
#define TC_TRANSACTION_RESP		0x14
#define SAFE_MODE_TYPE			0x15
#define SEND_EVENT				0x16
#define ASK_OBC_ALIVE			0x17
#define OBC_IS_ALIVE			0x18
#define SSM_ERROR_ASSERT		0x19
#define SSM_ERROR_REPORT		0x1A
#define ENTER_LOW_POWER_COM		0x1B
#define EXIT_LOW_POWER_COM		0x1C
#define ENTER_COMS_TAKEOVER_COM	0x1D
#define EXIT_COMS_TAKEOVER_COM	0x1E
#define PAUSE_OPERATIONS		0x1F
#define RESUME_OPERATIONS		0x20
#define LOW_POWER_MODE_ENTERED	0x21
#define LOW_POWER_MODE_EXITED	0x22
#define COMS_TAKEOVER_ENTERED	0x23
#define COMS_TAKEOVER_EXITED	0x24
#define OPERATIONS_PAUSED		0x25
#define OPERATIONS_RESUMED		0x26
#define OPEN_VALVES				0x27
#define COLLECT_PD				0x28
#define PD_COLLECTED			0x29

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
#define SHUNT_DPOT				0x11
#define COMS_TEMP				0x12
#define OBC_TEMP				0x13
#define PAY_TEMP0				0x14
#define PAY_TEMP1				0x15
#define PAY_TEMP2				0x16
#define PAY_TEMP3				0x17
#define PAY_TEMP4				0x18
#define PAY_HUM					0x19
#define PAY_PRESS				0x1A
#define PAY_ACCEL_X				0x1B
#define PAY_FL_PD0				0x1C
#define PAY_FL_PD1				0x1D
#define PAY_FL_PD2				0x1E
#define PAY_FL_PD3				0x1F
#define PAY_FL_PD4				0x20
#define PAY_FL_PD5				0x21
#define PAY_FL_PD6				0x22
#define PAY_FL_PD7				0x23
#define PAY_FL_PD8				0x24
#define PAY_FL_PD9				0x25
#define PAY_FL_PD10				0x26
#define PAY_FL_PD11				0x27
#define PAY_FL_OD_PD0			0x28
#define PAY_FL_OD_PD1			0x29
#define PAY_FL_OD_PD2			0x2A
#define PAY_FL_OD_PD3			0x2B
#define PAY_FL_OD_PD4			0x2C
#define PAY_FL_OD_PD5			0x2D
#define PAY_FL_OD_PD6			0x2E
#define PAY_FL_OD_PD7			0x2F
#define PAY_FL_OD_PD8			0x30
#define PAY_FL_OD_PD9			0x31
#define PAY_FL_OD_PD10			0x32
#define PAY_FL_OD_PD11			0x33
#define PAY_MIC_OD_PD0			0x34
#define PAY_MIC_OD_PD1			0x35
#define PAY_MIC_OD_PD2			0x36
#define PAY_MIC_OD_PD3			0x37
#define PAY_MIC_OD_PD4			0x38
#define PAY_MIC_OD_PD5			0x39
#define PAY_MIC_OD_PD6			0x3A
#define PAY_MIC_OD_PD7			0x3B
#define PAY_MIC_OD_PD8			0x3C
#define PAY_MIC_OD_PD9			0x3D
#define PAY_MIC_OD_PD10			0x3E
#define PAY_MIC_OD_PD11			0x3F
#define PAY_MIC_OD_PD12			0x40
#define PAY_MIC_OD_PD13			0x41
#define PAY_MIC_OD_PD14			0x42
#define PAY_MIC_OD_PD15			0x43
#define PAY_MIC_OD_PD16			0x44
#define PAY_MIC_OD_PD17			0x45
#define PAY_MIC_OD_PD18			0x46
#define PAY_MIC_OD_PD19			0x47
#define PAY_MIC_OD_PD20			0x48
#define PAY_MIC_OD_PD21			0x49
#define PAY_MIC_OD_PD22			0x4A
#define PAY_MIC_OD_PD23			0x4B
#define PAY_MIC_OD_PD24			0x4C
#define PAY_MIC_OD_PD25			0x4D
#define PAY_MIC_OD_PD26			0x4E
#define PAY_MIC_OD_PD27			0x4F
#define PAY_MIC_OD_PD28			0x50
#define PAY_MIC_OD_PD29			0x51
#define PAY_MIC_OD_PD30			0x52
#define PAY_MIC_OD_PD31			0x53
#define PAY_MIC_OD_PD32			0x54
#define PAY_MIC_OD_PD33			0x55
#define PAY_MIC_OD_PD34			0x56
#define PAY_MIC_OD_PD35			0x57
#define PAY_MIC_OD_PD36			0x58
#define PAY_MIC_OD_PD37			0x59
#define PAY_MIC_OD_PD38			0x5A
#define PAY_MIC_OD_PD39			0x5B
#define PAY_MIC_OD_PD40			0x5C
#define PAY_MIC_OD_PD41			0x5D
#define PAY_MIC_OD_PD42			0x5E
#define PAY_MIC_OD_PD43			0x5F
#define PAY_MIC_OD_PD44			0x60
#define PAY_MIC_OD_PD45			0x61
#define PAY_MIC_OD_PD46			0x62
#define PAY_MIC_OD_PD47			0x63
#define PAY_TEMP				0x64
#define PAY_ACCEL_Y				0x65
#define PAY_ACCEL_Z				0x66

/* VARIABLE NAMES		*/
#define MPPTX					0xFF
#define MPPTY					0xFE
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
#define BALANCE_L				0xF1
#define BALANCE_H				0xF0
#define SSM_CTT					0xEF
#define SSM_OGT					0xEE
#define OBC_CTT					0xED
#define OBC_OGT					0xEC
#define COMS_FDIR_SIGNAL		0xEB
#define EPS_FDIR_SIGNAL			0xEA
#define PAY_FDIR_SIGNAL			0xE9
#define BATT_HEAT				0xE8

/* Global variables to be used for CAN communication */
uint8_t	status, mob_number, send_now, send_hk, send_data, set_sens_h, set_sens_l, set_varf, ask_alive;
uint8_t enter_low_powerf, exit_low_powerf, enter_take_overf, exit_take_overf, pause_operationsf, resume_operationsf;
uint8_t read_response, write_response, open_valvesf, collect_pdf;
uint8_t receive_arr[8], send_arr[8], read_arr[8], write_arr[8], data_req_arr[8];
uint8_t sensh_arr[8], sensl_arr[8], setv_arr[8], pause_msg[8], resume_msg[8];
uint8_t id_array[6];	// Necessary due to the different mailbox IDs for COMS, EPS, PAYL.

#if (SELF_ID == 1)
/* Global Variables for EPS		*/
uint16_t pxv, pxi, pyv, pyi, battmv, battv, epstemp, shuntdpot, battin, battout, comsv, comsi, payv, payi, obcv, obci;
uint16_t pxv_high, pxv_low, pxi_high, pxi_low, pyv_high, pyv_low, pyi_high, pyi_low;
uint16_t battmv_high, battmv_low, battv_high, battv_low, battin_high, battin_low;
uint16_t battout_high, battout_low, epstemp_high, epstemp_low, comsv_high, comsv_low, comsi_high, comsi_low;
uint16_t payv_high, payv_low, payi_high, payi_low, obcv_high, obcv_low, obci_high, obci_low;
uint8_t mpptx, mppty, balance_h, balance_l, batt_heater_control;
uint16_t temp_old, press_old, acc_x_old, acc_y_old, acc_z_old;
#endif

// Global variable used to store the current minute (updated by a CAN message from the OBC)
uint8_t CURRENT_MINUTE;

// Global variables for the different modes that the SSM can be in.
uint8_t LOW_POWER_MODE;
uint8_t PAUSE;

/* Signal for FDIR Error Handling (SSM Loops on this) */
uint8_t ssm_fdir_signal;

uint8_t data0[DATA_BUFFER_SIZE];	// Data Buffer for MOb0
uint8_t data1[DATA_BUFFER_SIZE];	// Data Buffer for MOb1
uint8_t data2[DATA_BUFFER_SIZE];	// Data Buffer for MOb2
uint8_t data3[DATA_BUFFER_SIZE];	// Data Buffer for MOb3
uint8_t data4[DATA_BUFFER_SIZE];	// Data Buffer for MOb4
uint8_t data5[DATA_BUFFER_SIZE];	// Data Buffer for MOb5

uint8_t event_readyf;
uint8_t event_arr[8];

#if (SELF_ID == 0)
/* Global variables used for PUS packet communication */
uint8_t new_tm_msg[8], new_tc_msg[8], tm_sequence_count, new_tm_msgf, current_tm_fullf, tc_packet_readyf;
uint8_t tc_transfer_completef, start_tc_transferf, receiving_tmf;
uint8_t current_tm[PACKET_LENGTH], tm_to_downlink[PACKET_LENGTH], current_tc[PACKET_LENGTH];

// Global Flags and Constants for Coms TakeOver
uint8_t TAKEOVER;					// Coms is taking over for OBC
uint8_t REQUEST_TAKEOVER;			// Coms requests permission to takeover
uint8_t REQUEST_ALIVE_IN_PROG;		// A "are you alive?" message has been sent to OBC
uint8_t FAILED_COUNT;				// Consecutive times Coms has not received alive confirmation
uint32_t ISALIVE_COUNTER;			// Counts time since "are you alive?" sent and confirmation received
uint32_t MAX_WAIT_TIME;				//Max time Coms will wait for OBC response

/********** Global Variables for Transceiver Operations***************/
unsigned long previousTime;
unsigned long currentTime;
long int lastTransmit;
long int lastCycle;
long int lastAck;
long int lastToggle;
uint8_t tx_mode;
uint8_t rx_mode;
uint8_t rx_length;
uint8_t tx_length;
volatile uint32_t count32ms;
uint8_t new_packet[152];
uint8_t packet_receivedf;
uint8_t current_transceiver;
uint32_t countcycles;
uint8_t last_rx_packet_height;
uint8_t last_tx_packet_height;
uint32_t receiving_sequence_control;
uint32_t transmitting_sequence_control;
uint8_t test_reg[6];
uint8_t t_message[128];
uint8_t tx_fail_count;
uint8_t ack_acquired;
long int lastCalibration;
long int startedReceivingTM;
uint8_t low_half_acquired;

/* Global variables used for operational timeouts */
uint32_t ssm_ok_go_timeout;
uint8_t ssm_consec_trans_timeout;

/* Pointers to the packet list (for buffering) */
packet packet_list[5];
uint8_t packet_count;

#endif

/* Global variables to be used for CAN communication */
uint8_t msg_received;
uint8_t trans_msg [6];	// ** Use this to store the incoming message, just pick a fixed size.

uint8_t check;

/*****************************************************/
#endif

#if (SELF_ID == 2)
uint8_t pressure_calib[12];
#endif
