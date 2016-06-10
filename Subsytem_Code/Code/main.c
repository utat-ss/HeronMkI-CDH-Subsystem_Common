/*
    Author: Keenan Burnett
	***********************************************************************
	*	FILE NAME:		main.c
	*
	*	PURPOSE:		
	*	This is the main program which shall be run on the ATMEGA32M1s to be used on subsystem
	*	microcontrollers.
	*
	*	FILE REFERENCES:	io.h, interrupt, port.h, Timer.h, can_lib.h, adc_lib.h, can_api.h,
	*						spi_lib.h, trans_lib.h, commands.h, mppt_timer.h, battBalance.h,
	*						comsTimer.h, globa_var.h
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
	*	01/02/2015		Created.
	*
	*	02/06/2015		Edited the header.
	*
	*	02/28/2015		I am now editing the main program such that if command is received
	*					which requests data from the ADC, it will be sent back over CAN.
	*
	*	03/07/2015		I have written a file called can_api.c which allows me to clean up
	*					a lot of the code that was in main.c related to can. Now the main 
	*					program is much easier to understand and I will likely make less
	*					errors when developing code in the future.
	*
	*	04/05/2015		I am adding code in order to allow the subsystem micro to communicate
	*					with SPI devices. At this time, I am operating the subsystem micro as a 
	*					master and the OBC as a slave in order to test the subsystem's capabilities
	*					as an SPI master. Later, we will be using the ATMEGA32M1 for relaying 
	*					data from SPI sensors to the OBC.
	*
	*	04/30/2015		The SPI library that I wrote is now operational and is being used to communicate
	*					with the OBC (ATSAM3X8E) as its slave.
	*
	*	05/21/2015		I am adding a section which corresponds to checking whether the COMS transceiver
	*					has experience an RX buffer overflow and consequently reading an ASCII character
	*					and sending it to the OBC via CAN.
	*
	*	07/10/2015		I am working on getting the new SPI temperature sensor working with the 32M1. This
	*					SPI device has its own slave select == SS2 == PC5 == Pin 18 on the new PCB dev boards.
	*					Hence, instead of reading an analog temperature we will now be switching to digital.
	*
	*	07/12/2015		The new SPI sensor is now working and SPI temperature data is now being sent to the OBC
	*					as opposed to analog data. I am also working on getting the transceiver and SPI sensor
	*					to work at the same time on SPI.
	*
	*	08/02/2015	    I have added a couple files named commands.c and commands.h where we will store the 
	*					various different operations that the SSM can act upon when requested by the OBC or
	*					prompted by its own sensor readings. Hence, the series of 'if' statements that existed
	*					within the main while loop has now been replaced by run_commands().
	*
	*	09/20/2015		At this point, we want this program to be used for PAY, COMS, as well as EPS with the 
	*					the only difference being that SELF_ID is defined to different values. Hence if your
	*					SSM requires functionality specific only to it, you must use an if statement including SELF_ID.
	*
	*	11/12/2015		It seemed to make sense to lump all of my global variables and definitions in a place that
	*					was obvious to new people being introduced to the code. Hence we now have a global_var.h
	*					header for this. In addition, I created an initialization function init_global_vars()
	*					and put it in sys_init().
	*
	*	03/29/2016		COMS, EPS, PAY are now three functionally separate programs as I have made use of #if (SELF_ID == X)
	*					statements throughout this project. It is also much clearer now where the boundaries stand
	*					between each of the programs. Any new functionality which pertains to a single SSM should be
	*					contained within a similar statement.			
	*
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "port.h"
#include "Timer.h"
#include "can_lib.h"
#include "adc_lib.h"
#include "dac_lib.h"
#include "can_api.h"
#include "spi_lib.h"
#include "global_var.h"
#include "uart.h"
#if (SELF_ID == 0)
	#include "trans_lib.h"
	#include "comsTimer.h"
#endif
#include "commands.h"
#if (SELF_ID == 1)
	#include "mppt_timer.h"
	#include "battBalance.h"
	#include "sensors.h"
#endif
#if (SELF_ID == 2)
	#include "port_expander.h"
#endif

/* Function Prototypes for functions in this file */
static void io_init(void);
static void sys_init(void);
static void init_global_vars(void);
#if (SELF_ID == 2)
static void init_port_expander_pins(void);
#endif
/**************************************************/

volatile uint8_t CTC_flag;	// Variable used in timer.c

int main(void)
{		
	/* Initialize All Hardware and Interrupts */
	sys_init();
	//uint32_t count = 0;
	#if (SELF_ID == 0)
	{
		PIN_toggle(LED3);
		delay_ms(100);
		PIN_toggle(LED3);
		delay_ms(100);
		PIN_toggle(LED3);
		delay_ms(100);
		PIN_toggle(LED3);
		setup_fake_tc();
		uart_printf("*** RESET COMS ***\n\r");
	}
	#endif
	/*		Begin Main Program Loop					*/
	#if (SELF_ID) == 2
	uint16_t temperature;
	uint16_t acc_data = 0;
	uint32_t* temp_raw = malloc(sizeof(uint32_t));
	uint8_t sign = 1;
	uint8_t state;
		uart_printf("*** RESET PAY ***\n\r");
	#endif
	#if (SELF_ID == 1)
		uart_printf("*** RESET EPS ***\n\r");
	#endif
	while(1)
    {	
		/* Reset the WDT */
		wdt_reset();
		/* CHECK FOR A GENERAL INCOMING MESSAGE INTO MOB0 as well as HK into MOB5 */
		can_check_general();
		if(!PAUSE)
		{
			/*		TRANSCEIVER COMMUNICATION	*/
			#if (SELF_ID == 0)
				// If you are COMS, please check that receiving_tmf == 0 before
				// doing anything that is time-intensive (takes more than 10 ms).
				if(!receiving_tmf)
				{
					delay_ms(250);
					transceiver_run();
				}
				if(millis() - startedReceivingTM > TM_TIMEOUT)
					receiving_tmf = 0;
				// Continually check if COMS needs to takeover for OBC
				//check_obc_alive();
			#endif
			#if (SELF_ID == 1)
				delay_ms(250);
				//PIN_toggle(LED3);
				run_mppt();
				uint32_t sensor_data;
				//update_sensor(PANELY_I);
				//uart_printf("PANELY_I(MILIA)				:	+%u\n\r", pxi);
				//update_sensor(PANELY_V);
				//uart_printf("PANELY_V(MILIV)				:	+%u\n\r", pxv);
				//update_sensor(COMS_V);
				//uart_printf("COMS_V(MV)				:	+%u\n\r", comsv);
				//update_sensor(COMS_I);
				//uart_printf("COMS_I(MV)				:	+%u\n\r", comsi);
				update_sensor_all();
				//sensor_data = read_multiplexer_sensor(BATT_V_PIN);
				//uart_printf("PANELX_V(RAW)				:	+%lu\n\r", sensor_data);
				
				//test_reg[0] = *mux_result;
				//test_reg[1] = *(mux_result + 1);
				//send_can_value(test_reg);
				
				spi_send_shunt_dpot_value(0xB2);
			#endif
			#if (SELF_ID == 2)
				//pressure_sensor_init(pressure_calib);
				//accelerometer_init();
				collect_pressure();
				temperature = (uint32_t)spi_retrieve_temp(PAY_TEMP_CS);	// Get raw temp reading
				uart_printf("TEMP(C)			:	+%u\n\r", temperature);
				delay_ms(50);
				acc_data = spi_retrieve_acc(1);
				uart_printf("ACC (X)		:	+%d\n\r", acc_data);
				acc_data = spi_retrieve_acc(2);
				uart_printf("ACC (Y)		:	+%d\n\r", acc_data);
				acc_data = spi_retrieve_acc(3);
				uart_printf("ACC (Z)		:	+%d\n\r", acc_data);			
				delay_ms(500);
			#endif
		}		
		/*	EXECUTE OPERATIONS WHICH WERE REQUESTED */
		run_commands();
	}
}

static void sys_init(void) 
{
	/* Make sure sys clock is at least 8MHz */
	CLKPR = 0x80;
	CLKPR  = 0;
	//PIN_clr(LED1);
	
	/* Common Initialization */
	init_global_vars();
	io_init();
	#if (SELF_ID == 0)
		PIN_set(UHF_RST);
	#endif
	timer_init();
	adc_initialize();
	can_init(0);
	can_init_mobs();
	spi_initialize_master();
	//#if (SELF_ID != 0)
	uart_init();
	//#endif
	/* Enable watchdog timer - 2s */
	wdt_enable(WDTO_8S);
	
	/* COMS ONLY Initialization */
	#if (SELF_ID == 0)
		coms_timer_init();
	#endif

	/* EPS ONLY Initialization */
	#if (SELF_ID == 1)
		uart_sendmsg("*****BEGIN EPS INIT*****\n\r");
		// Ensure all SS bits set high
		PIN_clr(ANT_DEP_PIN);
		SS1_set_high(EPS_DPOT_CS);
		SS1_set_high(EPS_TEMP_CS);
		//PIN_set(2); // This is the SS pin, set high so the 32M1 can't become a slave
		
		mpptx = 0xC0;
		mppty = 0xC0;
		balance_l = 0;				// Turn off the low transistor (NPN)
		balance_h = 0;				// Turn off the high transistor (PNP) **THIS IS CURRENTLY AN ISSUE AS I NEED TO ADD AN INVERTER
		batt_heater_control = 0;	// Start with heaters off
		pxv = 0xBF;
		pxi	= 0x0F;
		pyv = 0x5F;
		pyi = 0x2F;
		shuntdpot = 0xAC;
		// Keenan says if I want to do this I have to wait for the global interrupts to be enabled
		//spi_send_shunt_dpot_value(0xAC);		// 0xAC should be the correct value because we are using the H and W so 0 Ohms = 0xFF
		adc_initialize();
		mppt_timer_init();
		uart_sendmsg("*****FINISH EPS INIT*****\n\r");
		PIN_set(LED1);	
	#endif

	/* PAY ONLY Initialization */
	#if (SELF_ID == 2)
		dac_initialize();
	#endif

	/* Enable global interrupts (required for timers) */
	sei();

	/* COMS ONLY Initialization */
	#if (SELF_ID == 0)
		//dac_initialize();
		//uint8_t dac_reg[2];
		//dac_reg[0] = 0xBA;
		//dac_reg[1] = 0x02;
		//dac_set(dac_reg);
		SS1_set_high(COMS_TEMP_SS);		// SPI Temp Sensor.	
		SS1_set_high(COMS_VHF_SS);
		SS1_set_low(COMS_UHF_SS);
		PIN_set(UHF_FE_EN);
		PIN_clr(UHF_FE_TR);
		PIN_clr(UHF_FE_BYP);
		transceiver_initialize();
	#endif
	
	/* PAY ONLY Initialization */
	#if (SELF_ID == 2)
		PIN_set(EXP_RST);
		for(uint8_t i = 0; i < 8; i++)		// Initializes config registers of Port Expanders themselves
		{
			port_expander_init(i);			
		}
		init_port_expander_pins();
		pressure_sensor_init(pressure_calib);
		accelerometer_init();
		//initialize_adc_all();
		//gpiob_pin_mode(0, 0, OUTPUT);
	#endif
}

static void io_init(void) 
{	
	/* Init PORTB[7:0] */
	DDRB = 0xFE;
	/* Init PORTC[7:0] */	// Note: PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0x13;
	PORTC = 0x00;
	/* Init PORTD[7:0] */
	DDRD = 0x0D;
	PORTD = 0x00;
	// Init PORTE[2:0] */
	DDRE = 0x00;
	PORTE = 0x00;
#if (SELF_ID == 0)
	if(PUS_COMMUNICATION_ON)
		DDRD = 0x6F;
	else
		DDRD = 0x63;	
	DDRC = 0x37;
#endif	
#if (SELF_ID == 1)
	// Init the EPS I/O (Set the pins that we want as outputs to act as outputs)
	DDRB = 0b11111110;	// SCK | bal l | bal h | s2 | s1 | batt_heat | MOSI | MISO
	DDRC = 0b11010111;	// s3 | s0 | Z | eps_temp | X | X | ANTENNA DEPLOY | RED LED
	if(MPPT_ENABLE)
		DDRD = 0b01101011;	// X | mppty | mpptx | X | SS/RXD | X | dpot_ss | BLUE LED
	else
		DDRD = 0b01100011;	// X | mppty | mpptx | X | SS | X | dpot_ss | BLUE LED	
#endif
#if (SELF_ID == 2)
	DDRC |= 0b11000100;
	DDRD = 0b00000010;
	//DDRD = 0x1F;
#endif

}

static void init_global_vars(void)
{	
	uint8_t i, j;
	#if (SELF_ID == 0)			// COMS Variable Initialization
		id_array[0] = SUB0_ID0;
		id_array[1] = SUB0_ID1;
		id_array[2] = SUB0_ID2;
		id_array[3] = SUB0_ID3;
		id_array[4] = SUB0_ID4;
		id_array[5] = SUB0_ID5;
		
		for (i = 0; i < 152; i++)		// Initialize the TM/TC Packet arrays.
		{
			current_tm[i] = 0;
			current_tc[i] = 0;
			tm_to_downlink[i] = i;
			new_packet[i] = 0;
		}
		for (i = 0; i < 8; i++)
		{
			new_tm_msg[i] = 0;
			new_tc_msg[i] = 0;		
		}

		/* Initialize Global coms takeover flags to zero */
		TAKEOVER = 0;
		REQUEST_TAKEOVER = 0;
		REQUEST_ALIVE_IN_PROG = 0;
		FAILED_COUNT = 0;
		ISALIVE_COUNTER = 0;
		MAX_WAIT_TIME = 18400;
	
		/* Initialize Operational Timeouts */
		ssm_ok_go_timeout = 1000;			// ~1000 ms
		ssm_consec_trans_timeout = 100;		// ~10 ms

		/* Transceiver Variables */
		previousTime = 0;
		currentTime = 0;
		lastTransmit = 0;
		lastCycle = 0;
		lastToggle = 0;
		tx_mode = 0;
		rx_mode = 1;
		rx_length = 0;
		tx_length = 0;
		count32ms = 0;
		packet_receivedf = 0;
		current_transceiver = 0;
		last_rx_packet_height = 0;
		last_tx_packet_height = 0xFF;
		receiving_sequence_control = 0;
		transmitting_sequence_control = 0;
		tx_fail_count = 0;
		ack_acquired = 0;
		lastCalibration = 0;
		current_transceiver = 0;
		lastAck = 0;
		low_half_acquired = 0;
		startedReceivingTM = 0;

		/* PUS Packet Variables */
		for(j = 0; j < 5; j++)
		{
			for(i = 0; i < 152; i++)
			{
				packet_list[j].data[i] = 0;
			}
		}
		for(i = 0; i < 77; i ++)
		{
			new_packet[i] = i;
			t_message[i] = i;
		}
		packet_count = 0;
		
		/* Command Flags */
		new_tm_msgf = 0;
		tm_sequence_count = 0;
		current_tm_fullf = 0;
		tc_packet_readyf = 0;
		tc_transfer_completef = 0;
		start_tc_transferf = 0;
		receiving_tmf = 0;
		ask_alive = 0;
		enter_take_overf = 0;
		exit_take_overf = 0;
		alert_deployf = 0;
	
	#endif
	#if (SELF_ID == 1)			// EPS Variable Initialization
		id_array[0] = SUB1_ID0;
		id_array[1] = SUB1_ID1;
		id_array[2] = SUB1_ID2;
		id_array[3] = SUB1_ID3;
		id_array[4] = SUB1_ID4;
		id_array[5] = SUB1_ID5;

		/* Dummy Values for Testing Housekeeping */
		pxv = 0x01;
		pxi = 0x02;
		pyv = 0x03;
		pyi = 0x04;
		battmv = 0x05;
		battv = 0x06;
		epstemp = 0x07;
		shuntdpot = 0x08;
		battin = 0x09;
		battout = 0x0A;
		comsv = 0x0B;
		comsi = 0x0C;
		payv = 0x0D;
		payi = 0x0E;
		obcv = 0x0F;
		obci = 0x10;
		
		/* Command Flags */
		enter_low_powerf = 0;
		exit_low_powerf = 0;
	
	#endif
	#if (SELF_ID == 2)			// PAY Variable Initialization
		id_array[0] = SUB2_ID0;
		id_array[1] = SUB2_ID1;
		id_array[2] = SUB2_ID2;
		id_array[3] = SUB2_ID3;
		id_array[4] = SUB2_ID4;
		id_array[5] = SUB2_ID5;
		
		/* Command Flags */
		open_valvesf = 0;
		collect_pdf = 0;
	#endif
	
	/* Common Variable Initialization */	
	for (i = 0; i < 8; i ++)
	{
		receive_arr[i] = 0;			// Reset the message array to zero after each message.
		send_arr[i] = 0;
		read_arr[i] = 0;
		write_arr[i] = 0;
		data_req_arr[i] = 0;
		sensh_arr[i] = 0;
		sensl_arr[i] = 0;
		setv_arr[i] = 0;
		event_arr[i] = 0;
		pause_msg[i] = 0;
		resume_msg[i] = 0;
	}

	/* Initialize Global Command Flags to zero */
	send_now = 0;
	send_hk = 0;
	send_data = 0;
	read_response = 0;
	write_response = 0;
	set_sens_h = 0;
	set_sens_l = 0;
	set_varf = 0;
	pause_operationsf = 0;
	resume_operationsf = 0;	
	deploy_antennaf = 0;
	turn_off_deployf = 0;
	event_readyf = 0;
	antenna_deployed = 0;

	/* Initialize Global Mode variables to zero */
	LOW_POWER_MODE = 0;
	PAUSE = 0;
	ssm_fdir_signal = 0;
		
	return;
}

#if (SELF_ID == 0)
void check_obc_alive(void) {
	if (!REQUEST_ALIVE_IN_PROG)
	{
		if (FAILED_COUNT <= 1)
		{
			// Send an "are you alive?" message to OBC through CAN
			ask_alive = 1;
			REQUEST_ALIVE_IN_PROG = 1;
		}
		// The OBC has failed to confirm it is alive twice consecutively.
		// Request permission to takeover from ground station.
		else {
			REQUEST_TAKEOVER = 1;
			TAKEOVER = 1;
		}
	}
	
	else if (ISALIVE_COUNTER > MAX_WAIT_TIME)
	{
		// Has not received confirmation in the maximum allowed time.
		// Increment the failed counter and consider the current
		// "are you alive?" request finished.
		FAILED_COUNT++;
		REQUEST_ALIVE_IN_PROG = 0;
	}
	//else, wait while the ISALIVE_COUNTER increments.
}
#endif

// This function is in charge with initializing the direction and level
// of pins on all the port expanders in the payload.
#if (SELF_ID == 2)
static void init_port_expander_pins(void)
{
	uint8_t pex_id = 0;
	uint8_t i;
	/* PORT EXPANDER 000 (*8 on INT PCB)	*/
	/* (The one with the sensors)			*/
	// Set the data direction to output.
	for(i = 0; i < 4; i++)
	{
		gpioa_pin_mode(pex_id, i, OUTPUT);
	}
	gpioa_pin_mode(pex_id, 7, INPUT);
	gpioa_pin_mode(pex_id, 6, INPUT);
	gpioa_pin_mode(pex_id, 5, INPUT);
	//gpioa_pin_mode(pex_id, 4, INPUT);		
	// Set the default output to high (off for a SPI select)
	for(i = 0; i < 4; i++)
	{
		set_gpioa_pin(pex_id, i);
	}
	port_expander_write(0, IODIR_BASE, 0xF0);
	
	/* PORT EXPANDER 001 (*4 on INT PCB)	*/
	/* (The one with heaters and valves)	*/
	pex_id = 4;
	// Set the data direction
	for(i = 0; i < 5; i++)
	{
		gpiob_pin_mode(pex_id, i, OUTPUT);
	}
	for(i = 0; i < 8; i++)
	{
		gpioa_pin_mode(pex_id, i, OUTPUT);
	}
	// Set the default pin level
	for(i = 0; i < 5; i++)
	{
		clr_gpiob_pin(pex_id, i);	// All heaters off initially.
	}
	for(i = 0; i < 8; i++)
	{
		clr_gpioa_pin(pex_id, i);	// All valves pin low initially.
	}
	return;
}
#endif
