/*
 * multiplexer.h
 *
 * Created: 1/9/2016 7:30:18 PM
 *  Author: Rahman
 */ 

#include "port.h"
#include "global_var.h"
#include "adc_lib.h"

#define MUX_PANELX_I 0
#define MUX_PANELX_V 1
#define MUX_PANELY_I 2
#define MUX_PANELY_V 3
#define MUX_DPOT_SAMPLE_V 4
#define MUX_COMS_V 5
#define MUX_COMS_I 6
#define MUX_BATTIN_I 8
#define MUX_BATTOUT_I 9
#define MUX_PAY_I 10
#define MUX_PAY_V 11
#define MUX_OBC_V 12
#define MUX_OBC_I 13
#define MUX_BATT_V 14
#define MUX_BATTM_V 15

void select_multiplexer_output(uint8_t A);
uint8_t* read_multiplexer_sensor(uint8_t sensor_id);