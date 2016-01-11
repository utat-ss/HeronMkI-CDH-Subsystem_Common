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

/* Currently for reference
SENSOR NAMES
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
