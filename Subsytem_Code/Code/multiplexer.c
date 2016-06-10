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
	PIN_clr(S0_P);
	PIN_clr(S1_P);
	PIN_clr(S2_P);
	PIN_clr(S3_P);
	if ( (A & 1) == 1) PIN_set(S0_P);
	if ( (A & 2) == 2) PIN_set(S1_P);
	if ( (A & 4) == 4) PIN_set(S2_P);
	if ( (A & 8) == 8) PIN_set(S3_P);
}

/***************************************************************/
/* Read 10-bit sensor data from multiplexer.                   */
/* Sets multiplexer to sensor_id (sensors.h)                   */
/***************************************************************/
uint16_t read_multiplexer_sensor(uint8_t sensor_id)
{
	uint8_t read_value[2];
	uint16_t ret_val = 0;
	select_multiplexer_output(sensor_id);
	delay_cycles(160);
	adc_read(read_value);
	ret_val = ((uint16_t)read_value[1]) << 8;
	ret_val += (uint16_t)read_value[0];
	return ret_val;
}
