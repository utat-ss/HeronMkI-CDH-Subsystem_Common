/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		trans_lib.h
	*
	*	PURPOSE:	This file contains the includes and definitions required by 
	*				trans_lib.c
	*
	*	FILE REFERENCES:		spi_lib.h
	*
	*	EXTERNAL VARIABLES:		None.
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	5/05/2015		Created.
	*
*/

#include "spi_lib.h"
#include "port.h"
#include "Timer.h"

/* Global variables to be used for CAN communication */
uint8_t msg_received; 
uint8_t trans_msg [6];	// ** Use this to store the incoming message, just pick a fixed size.

/*********** DEFINITIONS ******************/

//define crystal oscillator frequency to 32MHz
#define f_xosc 32000000;							// What is this used for?

// We have LED3, LED6, and LED7.
// We'll make those LEDs correspond to GREEN, RED, YELLOW.

// Define Command Strobes
#define SRES 0x30
#define SFSTXON 0x31
#define SXOFF 0x32
#define SCAL 0x33
#define SRX 0x34
#define STX 0x35
#define SIDLE 0x36
#define SAFC 0x37
#define SWOR 0x38
#define SPWD 0x39
#define SFRX 0x3A
#define SFTX 0x3B
#define SWORRST 0x3C
#define SNOP 0x3D

// Define Register Addresses
#define IOCFG3        0x0000
#define IOCFG2        0x0001
#define IOCFG1        0x0002
#define IOCFG0        0x0003
#define SYNC3         0x0004
#define SYNC2         0x0005
#define SYNC1         0x0006
#define SYNC0         0x0007
#define SYNC_CFG1     0x0008
#define SYNC_CFG0     0x0009
#define DEVIATION_M   0x000A
#define MODCFG_DEV_E  0x000B
#define DCFILT_CFG    0x000C
#define PREAMBLE_CFG1 0x000D
#define PREAMBLE_CFG0 0x000E
#define FREQ_IF_CFG   0x000F
#define IQIC          0x0010
#define CHAN_BW       0x0011
#define MDMCFG1       0x0012
#define MDMCFG0       0x0013

// Define RX/TX FIFO pointers 
#define RXFIRST           0xD2
#define TXFIRST           0xD3
#define RXLAST            0xD4
#define TXLAST            0xD5

/******************************************/

/****** FUNCTION PROTOTYPES ***************/

void transceiver_initialize(void);
uint8_t reg_read(uint8_t addr);
void reg_write(uint8_t addr, uint8_t data);
uint8_t reg_read2F(uint8_t addr);
void reg_write2F(uint8_t addr, uint8_t data);
void get_status(uint8_t *CHIP_RDYn, uint8_t *state);
uint8_t cmd_str(uint8_t addr);
uint8_t dir_FIFO_read(uint8_t addr);
void dir_FIFO_write(uint8_t addr, uint8_t data);
void set_CSn(uint8_t state);
void reg_write_bit(uint8_t reg, uint8_t n, uint8_t data);
void reg_write_bit2F(uint8_t reg, uint8_t n, uint8_t data);

