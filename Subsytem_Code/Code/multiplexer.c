/*
 * multiplexer.c
 *
 * Created: 1/9/2016 5:17:10 PM
 *  Author: Rahman Qureshi
 *  Notes: Currently untested but pins should be correctly matched now 
 */

#include "multiplexer.h"

/******************************************************************************/
/* Writes the lower 4 bits of A to A3, A2, A1, A0 of the ADG1606 */
/******************************************************************************/
void select_multiplexer_output(uint8_t A)
{
	if ( (A & 1) == 1) PIN_set(22);
	if ( (A & 2) == 1) PIN_set(23);
	if ( (A & 4) == 1) PIN_set(24);
	if ( (A & 8) == 1) PIN_set(25);
}

/***************************************************************/
/* Read 10-bit sensor data from multiplexer.                   */
/* Sets multiplexer to sensor_id (sensors.h)                   */
/***************************************************************/
uint8_t read_multiplexer_sensor(uint8_t sensor_id, uint8_t* high, uint8_t* low)
{
	uint8_t read_value[2];
	uint8_t status;
	select_multiplexer_output(sensor_id);
	adc_set_pin(9);
	adc_initialize();
	status = adc_read(read_value);
	*high = read_value[1];
	*low = read_value[0
	return status;
}
