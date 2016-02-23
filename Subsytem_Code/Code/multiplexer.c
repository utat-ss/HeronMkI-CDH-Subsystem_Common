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
void select_multiplexer_output(uint8_t sensor_id)
{
	switch(sensor_id)
	{
		case PANELX_I:		// 0
			PIN_clr(S0_P);
			PIN_clr(S1_P);
			PIN_clr(S2_P);
			PIN_clr(S3_P);
			break;
		case PANELX_V:		// 1
			PIN_set(S0_P);
			PIN_clr(S1_P);
			PIN_clr(S2_P);
			PIN_clr(S3_P);
			break;
		case PANELY_I:		// 2
			PIN_clr(S0_P);
			PIN_set(S1_P);
			PIN_clr(S2_P);
			PIN_clr(S3_P);
			break;
		case PANELY_V:		// 3
			PIN_set(S0_P);
			PIN_set(S1_P);
			PIN_clr(S2_P);
			PIN_clr(S3_P);
			break;
		case DPOT_SAMPLE_V:	// 4
			PIN_clr(S0_P);
			PIN_clr(S1_P);
			PIN_set(S2_P);
			PIN_clr(S3_P);
			break;
		case COMS_V:		// 5
			PIN_set(S0_P);
			PIN_clr(S1_P);
			PIN_set(S2_P);
			PIN_clr(S3_P);
			break;
		case COMS_I:		// 6
			PIN_clr(S0_P);
			PIN_set(S1_P);
			PIN_set(S2_P);
			PIN_clr(S3_P);
			break;
		case BATTIN_I:		// 8
			PIN_clr(S0_P);
			PIN_clr(S1_P);
			PIN_clr(S2_P);
			PIN_set(S3_P);
			break;
		case BATTOUT_I:		// 9
			PIN_set(S0_P);
			PIN_clr(S1_P);
			PIN_clr(S2_P);
			PIN_set(S3_P);
			break;
		case PAY_I:			// 10
			PIN_clr(S0_P);
			PIN_set(S1_P);
			PIN_clr(S2_P);
			PIN_set(S3_P);
			break;
		case PAY_V:			// 11
			PIN_set(S0_P);
			PIN_set(S1_P);
			PIN_clr(S2_P);
			PIN_set(S3_P);
			break;
		case OBC_V:			// 12
			PIN_clr(S0_P);
			PIN_clr(S1_P);
			PIN_set(S2_P);
			PIN_set(S3_P);
			break;
		case OBC_I:			// 13
			PIN_set(S0_P);
			PIN_clr(S1_P);
			PIN_set(S2_P);
			PIN_set(S3_P);
			break;
		case BATT_V:		// 14
			PIN_clr(S0_P);
			PIN_set(S1_P);
			PIN_set(S2_P);
			PIN_set(S3_P);
			break;
		case BATTM_V:		// 15
			PIN_set(S0_P);
			PIN_set(S1_P);
			PIN_set(S2_P);
			PIN_set(S3_P);
			break;
		default:			// 0 
			PIN_clr(S0_P);
			PIN_clr(S1_P);
			PIN_clr(S2_P);
			PIN_clr(S3_P);
			break;
	}
}

/***************************************************************/
/* Read 10-bit sensor data from multiplexer.                   */
/* Sets multiplexer to sensor_id (sensors.h)                   */
/***************************************************************/
int read_multiplexer_sensor(uint8_t sensor_id, uint8_t* result_container)
{
	int status;
	select_multiplexer_output(sensor_id);
	status = adc_read(result_container);
	return status;
}
