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
	*	1/19/2015		Added definitions for each of the register names in the CC1120.
	*
	*					I also added a macro which obtains the current count of milliseconds
	*					which have gone by.
	*
*/

#include "spi_lib.h"
#include "port.h"
#include "Timer.h"
#include "global_var.h"

/*********** DEFINITIONS ******************/

/* Packet Structure so that we can have a linked list */

#define STATUS_INTERVAL 1000
#define ACK_TIMEOUT 2000
#define TRANSCEIVER_CYCLE 250
#define DEVICE_ADDRESS 0xA5
#define REAL_PACKET_LENGTH 80

//define crystal oscillator frequency to 32MHz
#define f_xosc 32000000;							// What is this used for?

// We have LED3, LED6, and LED7.
// We'll make those LEDs correspond to GREEN, RED, YELLOW.

#define STDFIFO	0x3F

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

// The following are the states
#define STATEIDLE   0b000
#define STATERX     0b001
#define STATETX     0b010
#define STATERXERR  0b110
#define STATETXERR  0b111

/*	Regular Register Space	*/
#define IOCFG3			0x00
#define IOCFG2			0x01
#define IOCFG1			0x02
#define IOCFG0			0x03
#define SYNC3			0x04
#define SYNC2			0x05
#define SYNC1			0x06
#define SYNC0			0x07
#define SYNC_CFG1		0x08
#define SYNC_CFG0		0x09
#define DEVIATION_M		0x0A
#define MODCFG_DEV_E	0x0B
#define DCFILT_CFG		0x0C
#define PREAMBLE_CFG1	0x0D
#define PREAMBLE_CFG0	0x0E
#define FREQ_IF_CFG		0x0F
#define IQIC			0x10
#define CHAN_BW			0x11
#define MDMCFG1			0x12
#define MDMCFG0			0x13
#define SYMBOL_RATE2	0x14
#define SYMBOL_RATE1	0x15
#define SYMBOL_RATE0	0x16
#define AGC_REF			0x17
#define	AGC_CS_THR		0x18
#define AGC_GAIN_ADJUST	0x19
#define AGC_CFG3		0x1A
#define AGC_CFG2		0x1B
#define AGC_CFG1		0x1C
#define AGC_CFG0		0x1D
#define FIFO_CFG		0x1E
#define DEV_ADDR		0x1F
#define SETTLING_CFG	0x20
#define FS_CFG			0x21
#define WOR_CFG1		0x22
#define WOR_CFG0		0x23
#define WOR_EVENT0_MSB	0x24
#define WOR_EVENT0_LSB	0x25
#define PKT_CFG2		0x26
#define PKT_CFG1		0x27
#define PKT_CFG0		0x28
#define RFEND_CFG1		0x29
#define RFEND_CFG0		0x2A
#define PA_CFG2			0x2B
#define PA_CFG1			0x2C
#define PA_CFG0			0x2D
#define PKT_LEN			0x2E

/*	Extended Register Space	*/
#define IF_MIX_CFG		0x00
#define FREQOFF_CFG		0x01
#define TOC_CFG			0x02
#define MARC_SPARE		0x03
#define ECG_CFG			0x04
#define CFM_DATA_CFG	0x05
#define EXT_CTRL		0x06
#define RCCAL_FINE		0x07
#define RCCAL_COARSE	0x08
#define RCCAL_OFFSET	0x09
#define FREQOFF1		0x0A
#define FREQOFF0		0x0B
#define FREQ2			0x0C
#define FREQ1			0x0D
#define FREQ0			0x0E
#define IF_ADC2			0x0F
#define IF_ADC1			0x10
#define IF_ADC0			0x11
#define FS_DIG1			0x12
#define FS_DIG0			0x13
#define FS_CAL3			0x14
#define FS_CAL2			0x15
#define FS_CAL1			0x16
#define FS_CAL0			0x17
#define FS_CHP			0x18
#define FS_DIVTWO		0x19
#define FS_DSM1			0x1A
#define FS_DSM0			0x1B
#define FS_DVC1			0x1C
#define FS_DVC0			0x1D
#define FS_LBI			0x1E
#define FS_PFD			0x1F
#define FS_PRE			0x20
#define FS_REG_DIV_CML	0x21
#define FS_SPARE		0x22
#define FS_VCO4			0x23
#define FS_VCO3			0x24
#define FS_VCO2			0x25
#define FS_VCO1			0x26
#define FS_VCO0			0x27
#define GBIAS6			0x28
#define GBIAS5			0x29
#define GBIAS4			0x2A
#define GBIAS3			0x2B
#define GBIAS2			0x2C
#define GBIAS1			0x2D
#define GBIAS0			0x2E
#define IFAMP			0x2F
#define LNA				0x30
#define RXMIX			0x31
#define XOSC5			0x32
#define XOSC4			0x33
#define XOSC3			0x34
#define XOSC2			0x35
#define XOSC1			0x36
#define XOSC0			0x37
#define ANALOG_SPARE	0x38
#define PA_CFG3			0x39
// Registers below do not have retention.
#define WOR_TIME1		0x64
#define WOR_TIME0		0x65
#define WOR_CAPTURE1	0x66
#define WOR_CAPTURE0	0x67
#define BIST			0x68
#define DCFILTOFFSET_I1	0x69
#define DCFILTOFFSET_I0	0x6A
#define DCFILTOFFSET_Q1	0x6B
#define DCFILTOFFSET_Q0	0x6C
#define IQIE_I1			0x6D
#define IQIE_I0			0x6E
#define IQIE_Q1			0x6F
#define IQIE_Q0			0x70
#define RSSI1			0x71
#define RSSI0			0x72
#define MARCSTATE		0x73
#define LQI_VAL			0x74
#define PQT_SYNC_ERR	0x75
#define DEM_STATUS		0x76
#define FREQOFF_EST1	0x77
#define FREQOFF_EST0	0x78
#define AGC_GAIN3		0x79
#define AGC_GAIN2		0x7A
#define AGC_GAIN1		0x7B
#define AGC_GAIN0		0x7C
#define CFM_RX_DATA_OUT	0x7D
#define CFM_TX_DATA_IN	0x7E
#define ASK_SOFT_RX_DATA	0x7F
#define RNDGEN			0x80
#define MAGN2			0x81
#define MAGN1			0x82
#define MAGN0			0x83
#define ANG1			0x84
#define ANG0			0x85
#define CHFILT_I2		0x86
#define CHFILT_I1		0x87
#define CHFILT_I0		0x88
#define CHFILT_Q2		0x89
#define CHFILT_Q1		0x8A
#define CHFILT_Q0		0x8B
#define GPIO_STATUS		0x8C
#define FSCAL_CTRL		0x8D
#define PHASE_ADJUST	0x8E
#define PARTNUMBER		0x8F
#define PARTVERSION		0x90
#define SERIAL_STATUS	0x91
#define MODEM_STATUS1	0x92
#define MODEM_STATUS0	0x93
#define MARC_STATUS1	0x94
#define MARC_STATUS0	0x95
#define PA_IFAMP_TEST	0x96
#define FSRF_TEST		0x97
#define PRE_TEST		0x98
#define PRE_OVR			0x99
#define ADC_TEST		0x9A
#define DVC_TEST		0x9B
#define ATEST			0x9C
#define ATEST_LVDS		0x9D
#define ATEST_MODE		0x9E
#define XOSC_TEST1		0x9F
#define XOSC_TEST0		0xA0
#define NUM_TXBYTES		0xD6
#define NUM_RXBYTES		0xD7
#define FIFO_NUM_TXBYTES	0xD8
#define FIFO_NUM_RXBYTES	0xD9

/*	Define RX/TX FIFO pointers */
#define RXFIRST           0xD2
#define TXFIRST           0xD3
#define RXLAST            0xD4
#define TXLAST            0xD5

/* Macro Definitions			*/
#define millis() (countcycles>>13)

//uint8_t new_packet[77];		// 76B data + 1B length

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
void reg_write_bit(uint8_t reg, uint8_t n, uint8_t data);
void reg_write_bit2F(uint8_t reg, uint8_t n, uint8_t data);
void transceiver_send(uint8_t* message, uint8_t address, uint8_t length);
void trans_check(void);
void reg_settings(void);
void prepareAck(void);
void transceiver_run(void);
void clear_new_packet(void);
uint8_t store_new_packet(void);
void load_packet_to_current_tc(void);
static void clear_current_tc(void);
void load_packet(void);
