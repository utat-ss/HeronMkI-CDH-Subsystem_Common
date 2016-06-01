/*
 * multiplexer.h
 *
 * Created: 1/9/2016 7:30:18 PM
 *  Author: Rahman
 */ 
#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "port.h"
#include "global_var.h"
#include "adc_lib.h"
#include "sensors.h"

void select_multiplexer_output(uint8_t A);
uint16_t read_multiplexer_sensor(uint8_t sensor_id);

#endif
