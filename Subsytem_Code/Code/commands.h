/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		commands.h
	*
	*	PURPOSE:	This program contains all includes and function prototypes required by commands.c
	*
	*	FILE REFERENCES:		can_api.h
	*
	*	EXTERNAL VARIABLES:		send_now, send_data, send_hk, send_coms
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:	None.
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	08/2/2015		Created.
	*
	*	08/08/2015		Added functions send_read_response() and send_write_response().
	*
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#include "can_api.h"
#include "Timer.h"
#include <avr/wdt.h>
#include "can_lib.h"
#include "global_var.h"
#if (SELF_ID == 0)
	#include "trans_lib.h"
#endif
#include "error_handling.h"
#include <stdint.h>
#include <stdlib.h>
#include "spi_lib.h"
#include "adc_lib.h"

/* Function Prototypes								 */	
void run_commands(void);
void send_response(void);
void send_housekeeping(void);
void send_sensor_data(void);
void send_coms_packet(void);
void send_read_response(void);
void send_write_response(void);
void set_sensor_high(void);
void set_sensor_low(void);
void set_var(void);
void receive_tm_msg(void);
void alert_obc_tcp_ready(void);
void send_pus_packet_tc(void);
void send_event(void);
void send_ask_alive(void);
void enter_low_power(void);
void exit_low_power(void);
void enter_take_over(void);
void exit_take_over(void);
void open_valves(void);
void collect_pd(void);
void pause_operations(void);
void resume_operations(void);
uint16_t collect_pressure(void);
uint8_t convert_to_temp(uint32_t* temp);
void alert_deploy(void);
void deploy_antenna(void);

/*****************************************************/
#endif
