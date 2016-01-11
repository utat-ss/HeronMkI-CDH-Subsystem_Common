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
	update_sensor(PANELX_V);
	update_sensor(PANELY_I);
	update_sensor(PANELY_V);
	update_sensor(BATTM_V);
	update_sensor(BATT_V);
	update_sensor(BATTIN_I);
	update_sensor(BATTOUT_I);
	update_sensor(EPS_TEMP);
	update_sensor(SHUNT_DPOT);
	update_sensor(COMS_V);
	update_sensor(COMS_I);
	update_sensor(PAY_V);
	update_sensor(PAY_I);
	update_sensor(OBC_V);
	update_sensor(OBC_I);
}

/************************************************************************/
// READ SPI SENSOR
//
// @param: sensor_name this is the name of the sensor as defined in global_var.h
// @NOTE: This function is currently a placeholder until I figure out all of the SPI stuff
/************************************************************************/
uint8_t* read_spi_sensor(uint8_t sensor_name)
{
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
	if(sensor_name == PANELX_V)
	{
		pxv = read_multiplexer_sensor(sensor_name);
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
		epstemp = read_spi_sensor(sensor_name);
	}
	if(sensor_name == SHUNT_DPOT)
	{
		shuntdpot = read_spi_sensor(sensor_name);
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
}