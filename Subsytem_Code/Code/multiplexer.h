/*
 * multiplexer.h
 *
 * Created: 1/9/2016 7:30:18 PM
 *  Author: Rahman
 */ 

#include "port.h"
#include "global_var.h"
#include "adc_lib.h"
#include "sensors.h"

void select_multiplexer_output(uint8_t sensor_id);
int read_multiplexer_sensor(uint8_t sensor_id, uint8_t* result_container);