/*
	Author: Samantha Murray

	***********************************************************************
	*	FILE NAME:		sensors.c
	*
	*	PURPOSE:	This program contains functions related to dealing with sensor reads from the ATMEGA32M1.
	*
	*	FILE REFERENCES:		sensors.h
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
*/

#include "sensors.h"
/************************************************************************/
// UPDATE_SENSOR_ALL
// 
// @NOTE: This will update the current value of all attached sensors 
/************************************************************************/
void update_sensor_all(void)
{
	update_sensor(PANELX_I);
	uart_printf("PANELX_I(MILIA)			:	+%u\n\r", pxi);
	update_sensor(PANELX_V);
	uart_printf("PANELX_V(MILIV)			:	+%u\n\r", pxv);
	update_sensor(PANELY_I);
	uart_printf("PANELY_I(MILIA)			:	+%u\n\r", pyi);
	update_sensor(PANELY_V);
	uart_printf("PANELY_V(MILIV)			:	+%u\n\r", pyv);
	update_sensor(BATT_V);
	uart_printf("BATT_V(MILIV)				:	+%u\n\r", battv);
	update_sensor(BATTIN_I);
	uart_printf("BATTIN_I(MILIA)			:	+%u\n\r", battin);
	update_sensor(BATTOUT_I);
	uart_printf("BATTOUT_I(MILIA)			:	+%u\n\r", battout);
	update_sensor(EPS_TEMP);
	uart_printf("EPS_TEMP(C)				:	+%u\n\r", epstemp);
	update_sensor(COMS_V);
	uart_printf("COMS_V(MILIV)				:	+%u\n\r", comsv);
	update_sensor(COMS_I);
	uart_printf("COMS_I(MILIA)				:	+%u\n\r", comsi);
	update_sensor(PAY_V);
	uart_printf("PAY_V(MILIV)				:	+%u\n\r", payv);
	update_sensor(PAY_I);
	uart_printf("PAY_I)(MILIA)				:	+%u\n\r", payi);
	update_sensor(OBC_V);
	uart_printf("OBC_V(MILIV)				:	+%u\n\r", obcv);
	update_sensor(OBC_I);
	uart_printf("OBC_I(MILIA)				:	+%u\n\r", obci);
	return;
}

/************************************************************************/
// UPDATE SENSOR
//
// @param: sensor_name this is the name of the sensor as defined in global_var.h
// @NOTE: This will update the current value of the attached sensor specified
/************************************************************************/
void update_sensor(uint8_t sensor_name)
{
	uint32_t analog = 0;
	if(sensor_name == PANELX_V)
	{
		analog = (uint32_t)read_multiplexer_sensor(sensor_name);
		analog *= 3300;
		analog /= 1024;
		analog *= PXV_MULTIPLIER;
		analog /= 1000;
		pxv = (uint16_t)analog
	}
	if(sensor_name == PANELX_I)
	{
		pxi = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == PANELY_V)
	{
		pyv = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == PANELY_I)
	{
		pyi  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == BATTM_V)
	{
		battmv  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == BATT_V)
	{
		battv  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == BATTIN_I)
	{
		battin  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == BATTOUT_I)
	{
		battout  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == EPS_TEMP)
	{
		epstemp = spi_retrieve_temp(EPS_TEMP_CS);
	}
	if(sensor_name == COMS_V)
	{
		comsv  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == COMS_I)
	{
		comsi  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == PAY_V)
	{
		payv  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == PAY_I)
	{
		payi  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == OBC_V)
	{
		obcv  = read_multiplexer_sensor(sensor_name);
	}
	if(sensor_name == OBC_I)
	{
		obci  = read_multiplexer_sensor(sensor_name);
	}
	return;
}
