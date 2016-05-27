/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		sensors.h
	*
	*	PURPOSE:	This program contains includes and definitions related to adc_lib.c
	*	
	*	FILE REFERENCES:		global_var.h
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
	*	01/10/2016		Created.
	*
*/
#include "global_var.h"
#include "spi_lib.h"

/* Correspond to pin connections to ADG1606 (pinNumber - 1) */
#define PANELX_I 0
#define PANELX_V 1
#define PANELY_I 2
#define PANELY_V 3
#define DPOT_SAMPLE_V 4
#define COMS_V 5
#define COMS_I 6
#define BATTIN_I 8
#define BATTOUT_I 9
#define PAY_I 10
#define PAY_V 11
#define OBC_V 12
#define OBC_I 13
#define BATT_V 14
#define BATTM_V 15

/* Multipliers for the EPS analog sensors (calculated for the IT5 Board) */

// Voltages
#define PXV_MULTIPLIER		1810
#define PYV_MULTIPLIER		1810
#define BATT_V_MULTIPLIER	2550
#define COMS_V_MULTIPLIER	1273
#define PAY_V_MULTIPLIER	1273
#define OBC_V_MULTIPLIER	1273

// Currents
#define PXI_MULTIPLIER		1810

/* Currently for reference
VARIABLE NAMES
#define MPPTX					0xFF
#define MPPTY					0xFE
#define EPS_MODE				0xFC
#define BALANCE_L				0xF1
#define BALANCE_H				0xF0
#define EPS_FDIR_SIGNAL			0xEA

Global Variables for EPS
uint16_t pxv_high, pxv_low, pxi_high, pxi_low, pyv_high, pyv_low, pyi_high, pyi_low;
uint16_t battmv_high, battmv_low, battv_high, battv_low, batti_high, batti_low, battemp_high;
uint16_t battemp_low, epstemp_high, epstemp_low, comsv_high, comsv_low, comsi_high, comsi_low;
uint16_t payv_high, payv_low, payi_high, payi_low, obcv_high, obcv_low, obci_high, obci_low;
uint8_t mpptx, mppty, balance_h, balance_l;
*/

void update_sensor_all(void);
void update_sensor(uint8_t sensor_name);
