/*
 * multiplexer.c
 *
 * Created: 1/9/2016 5:17:10 PM
 *  Author: Rahman Qureshi
 */ 

/********
	THIS CURRENTLY WON'T WORK BECAUSE I NEED TO FIX THE MAPPING 
	BETWEEN THE SENSOR IDS AND THE SENSOR NAMES
	
	LEFT FOR COMPILING PURPOSES
	*****************************/

#include "multiplexer.h"

/************************************************************************/
/* Writes the lower 4 bits of a to A3, A2, A1, A0 of the ADG1606        */
/************************************************************************/
void select_multiplexer_output(uint8_t A)
{
	if ( (A & 1) == 1) PIN_set(22);
	if ( (A & 2) == 1) PIN_set(23);
	if ( (A & 4) == 1) PIN_set(24);
	if ( (A & 8) == 1) PIN_set(25);
}

uint8_t* read_multiplexer_sensor(uint8_t sensor_id)
{
	uint8_t* read_value;
	select_multiplexer_output(sensor_id);
	adc_set_pin(9);
	adc_read(read_value);
	return read_value;
}
