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
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "port.h"
#include "Timer.h"
#include "can_lib.h"
#include "adc_lib.h"
#include "can_api.h"
#include "spi_lib.h"
#include "trans_lib.h"
#include "commands.h"
#include "mppt_timer.h"
#include "battBalance.h"
#include "comsTimer.h"
#include "global_var.h"

/* Function Prototypes for functions in this file */
static void io_init(void);
static void sys_init(void);
static void init_global_vars(void);
/**************************************************/

volatile uint8_t CTC_flag;	// Variable used in timer.c

int main(void)
{		
	uint8_t	i = 0;
	
	uint8_t msg_low = 0, msg_high = 0;
	
	uint8_t high = 0, low = 0;
	
	uint8_t msg = 0x66;
	
	uint8_t* adc_result;
	*adc_result = 0;

	// Initialize I/O, Timer, ADC, CAN, WDT, and SPI
	sys_init();
	
	/*		Begin Main Program Loop					*/	
    while(1)
    {	
		/* Reset the WDT */
		wdt_reset();
		
		/* CHECK FOR A GENERAL INCOMING MESSAGE INTO MOB0 as well as HK into MOB5 */
		can_check_general();
		
		/*		TRANSCEIVER COMMUNICATION	*/
		if(SELF_ID == 0)
		{
			// If you are COMS, please check that receiving_tmf == 0 before
			// doing anything that is time-intensive (takes more than 10 ms).
			if(!receiving_tmf)
				trans_check();		// Check for incoming packets.	
			//check_obc_alive();
		}
		if(SELF_ID == 1)
		{
			run_mppt();
			run_battBalance();
		}
		
		/*	EXECUTE OPERATIONS WHICH WERE REQUESTED */
		run_commands();
	}
}

static void sys_init(void) 
{
	// Make sure sys clock is at least 8MHz
	CLKPR = 0x80;  
	CLKPR = 0x00;
	
	init_global_vars();
	io_init();	
	timer_init();
	adc_initialize();
	can_init(0);
	can_init_mobs();
	spi_initialize_master();
	
	//enable watchdog timer - 2 second reset time approximate
	//WDTON Fuse has to be 1 for system reset mode - how do you do that?
	wdt_enable(WDTO_2S);

	/* Enable the timer for MMPT */
	if(SELF_ID == 1)
	{
		PIN_set(LED1);
		mppt_timer_init();
		mpptx = 0x3F;
		mppty = 0x1F;
		balance_l = 1;
		balance_h = 1;
		pxv = 0xBF;
		pxi	= 0x0F;
		pyv = 0x5F;
		pyi = 0x2F;
	}
	
	// Enable global interrupts for Timer execution
	sei();
	
	if (SELF_ID == 0)
	{
		transceiver_initialize();
		coms_timer_init();
	}

	SS1_set_high();		// SPI Temp Sensor.
	
	if(SELF_ID != 1)
	{
		PIN_toggle(LED1);
	}

}

static void io_init(void) 
{	
	// Init PORTB[7:0] // LED port
	DDRB = 0xFE;
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0x11;		// PC4 == SS1 for SPI_TEMP
	PORTC = 0x00;
	
	// Init PORTD[7:0]
	DDRD = 0x09;		// PD3 is the SS for SPI communications.
	PORTD = 0x01;		// PD3 should only go low during an SPI message.
	
	// Init PORTE[2:0]
	DDRE = 0x00;
	PORTE = 0x00;
}

static void init_global_vars(void)
{	
	uint8_t i;
	if (SELF_ID == 0)
	{
		id_array[0] = SUB0_ID0;
		id_array[1] = SUB0_ID1;
		id_array[2] = SUB0_ID2;
		id_array[3] = SUB0_ID3;
		id_array[4] = SUB0_ID4;
		id_array[5] = SUB0_ID5;
	}
	if(SELF_ID == 1)
	{
		id_array[0] = SUB1_ID0;
		id_array[1] = SUB1_ID1;
		id_array[2] = SUB1_ID2;
		id_array[3] = SUB1_ID3;
		id_array[4] = SUB1_ID4;
		id_array[5] = SUB1_ID5;
	}
	if(SELF_ID == 2)
	{
		id_array[0] = SUB2_ID0;
		id_array[1] = SUB2_ID1;
		id_array[2] = SUB2_ID2;
		id_array[3] = SUB2_ID3;
		id_array[4] = SUB2_ID4;
		id_array[5] = SUB2_ID5;
	}
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
		new_tm_msg[i] = 0;
		new_tc_msg[i] = 0;
		event_arr[i] = 0;
	}
	for (i = 0; i < 152; i++)		// Initialize the TM/TC Packet arrays.
	{
		current_tm[i] = 0;
		current_tc[i] = 0;
		tm_to_downlink[i] = 0;
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
	new_tm_msgf = 0;
	tm_sequence_count = 0;
	current_tm_fullf = 0;
	tc_packet_readyf = 0;
	tc_transfer_completef = 0;
	start_tc_transferf = 0;
	receiving_tmf = 0;
	event_readyf = 0;
	
	return;
}
