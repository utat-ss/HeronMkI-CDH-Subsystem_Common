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
	*
	*	05/27/2016		Got rid of sensor values we don't care about, update retrieval of eps_temp
	*					added in printing values to UART.
*/

#include "sensors.h"
#include "global_var.h"
#if (SELF_ID == 1)
/************************************************************************/
// UPDATE_SENSOR_ALL
// 
// @NOTE: This will update the current value of all attached sensors 
/************************************************************************/
void update_sensor_all(void)
{
	uart_printf("****NEW SENSOR COLLECTION***\n\r", pxi);
	update_sensor(PANELX_I);
	uart_printf("PANELX_I(MILIA)				:	+%u\n\r", pxi);
	update_sensor(PANELX_V);
	uart_printf("PANELX_V(MILIV)				:	+%u\n\r", pxv);
	update_sensor(PANELY_I);
	uart_printf("PANELY_I(MILIA)				:	+%u\n\r", pyi);
	update_sensor(PANELY_V);
	uart_printf("PANELY_V(MILIV)				:	+%u\n\r", pyv);
	update_sensor(BATT_V);
	uart_printf("BATT_V(MILIV)				:	+%u\n\r", battv);
	update_sensor(BATTIN_I);
	uart_printf("BATTIN_I(MILIA)				:	+%u\n\r", battin);
	delay_ms(250);
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
	//delay_ms(250);
	if(sensor_name == PANELX_V)
	{
		analog = (uint32_t)read_multiplexer_sensor(PANELX_V_PIN);
		uart_printf("PANELX_V(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= PXV_MULTIPLIER;
		analog /= 1000;
		analog -= 121;
		if(analog > 10000)
			analog = 0;
		pxv = (uint16_t)analog;
	}
	if(sensor_name == PANELX_I)
	{
		analog = (uint32_t)read_multiplexer_sensor(PANELX_I_PIN);
		uart_printf("PANELX_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= PXI_MULTIPLIER;
		analog -= 11;
		if(analog > 10000)
		analog = 0;
		pxi = (uint16_t)analog;
	}
	if(sensor_name == PANELY_V)
	{
		analog = (uint32_t)read_multiplexer_sensor(PANELY_V_PIN);
		uart_printf("PANELY_V(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= PYV_MULTIPLIER;
		analog /= 1000;
		analog -= 119;
		if(analog > 10000)
		analog = 0;
		pyv = (uint16_t)analog;
	}
	if(sensor_name == PANELY_I)
	{
		analog = (uint32_t)read_multiplexer_sensor(PANELY_I_PIN);
		uart_printf("PANELY_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= PYI_MULTIPLIER;
		analog -= 11;
		if(analog > 10000)
		analog = 0;
		pyi = (uint16_t)analog;
	}
	if(sensor_name == BATT_V)
	{
		analog = (uint32_t)read_multiplexer_sensor(BATT_V_PIN);
		uart_printf("BATT_V(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= BATT_V_MULTIPLIER;
		analog /= 1000;
		analog -= 398;
		if(analog > 10000)
		analog = 0;
		battv = (uint16_t)analog;		
	}
	if(sensor_name == BATTIN_I)
	{
		analog  = (uint32_t)read_multiplexer_sensor(BATTIN_I_PIN);
		uart_printf("BATTIN_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= BATTIN_MULTIPLIER;
		analog -= 119;
		if(analog > 10000)
		analog = 0;
		battin = (uint16_t)analog;
	}
	if(sensor_name == BATTOUT_I)
	{
		analog  = (uint32_t)read_multiplexer_sensor(BATTOUT_I_PIN);
		uart_printf("BATTOUT_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= BATTOUT_MULTIPLIER;
		analog -= 119;
		if(analog > 10000)
		analog = 0;
		battout = (uint16_t)analog;
	}
	if(sensor_name == EPS_TEMP)
	{
		epstemp = spi_retrieve_temp(EPS_TEMP_CS);
	}
	if(sensor_name == COMS_V)
	{
		analog  = (uint32_t)read_multiplexer_sensor(COMS_V_PIN);
		uart_printf("COMS_V(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= COMS_V_MULTIPLIER;
		analog /= 1000;
		analog -= 232;
		if(analog > 10000)
		analog = 0;
		comsv = (uint16_t)analog;
	}
	if(sensor_name == COMS_I)
	{
		analog  = (uint32_t)read_multiplexer_sensor(COMS_I_PIN);
		uart_printf("COMS_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= COMS_I_MULTIPLIER;
		analog -= 118;
		if(analog > 10000)
		analog = 0;
		comsi = (uint16_t)analog;
	}
	if(sensor_name == PAY_V)
	{
		analog = (uint32_t)read_multiplexer_sensor(PAY_V_PIN);
		uart_printf("PAY_V(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= PAY_V_MULTIPLIER;
		analog /= 1000;
		analog -= 238;
		if(analog > 10000)
		analog = 0;
		payv = (uint16_t)analog;
	}
	if(sensor_name == PAY_I)
	{
		analog  = (uint32_t)read_multiplexer_sensor(PAY_I_PIN);
		uart_printf("PAY_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= PAY_I_MULTIPLIER;
		analog -= 124;
		if(analog > 10000)
		analog = 0;
		payi = (uint16_t)analog;
	}
	if(sensor_name == OBC_V)
	{
		analog  = (uint32_t)read_multiplexer_sensor(OBC_V_PIN);
		uart_printf("OBC_V(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= OBC_V_MULTIPLIER;
		analog /= 1000;
		analog -= 239;
		if(analog > 10000)
		analog = 0;
		obcv = (uint16_t)analog;
	}
	if(sensor_name == OBC_I)
	{
		analog  = (uint32_t)read_multiplexer_sensor(OBC_I_PIN);
		uart_printf("OBC_I(RAW)				:	+%lu\n\r", analog);
		analog *= 3300;
		analog /= 1024;
		analog *= 500000;
		analog /= OBC_I_MULTIPLIER;
		analog -= 530;
		if(analog > 500)
		analog = 37;
		obci = (uint16_t)analog;
	}
	return;
}

#endif
