/*
    Author: Keenan Burnett
	(Originally taken from Digi-Key Design Support)

	***********************************************************************
	*	FILE NAME:		main.c
	*
	*	PURPOSE:		
	*	This is the main program which shall be run on the ATMEGA32M1s to be used on subsystem
	*	microcontrollers.
	*
	*	FILE REFERENCES:	io.h, interrupt, LED.h, Timer.h, can_lib.h, adc_lib.h, can_api.h,
	*						spi_lib.h, trans_lib.h
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
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "LED.h"
#include "Timer.h"
#include "can_lib.h"
#include "adc_lib.h"
#include "can_api.h"
#include "spi_lib.h"
#include "trans_lib.h"

/* Function Prototypes for functions in this file */
static void io_init(void);
static void sys_init(void);
/**************************************************/

volatile uint8_t CTC_flag;	// Variable used in timer.c

int main(void)
{		
	uint8_t	i = 0;
	
	uint8_t msg_low = 0, msg_high = 0;
	
	uint8_t high = 0, low = 0;

	// Initialize I/O, Timer, ADC, CAN, and SPI
	sys_init();
	
	/*		Begin Main Program Loop					*/	
    while(1)
    {		
		/* CHECK FOR A GENERAL INCOMING MESSAGE INTO MOB0 */
		can_check_general();
		
		/* CHECK FOR HOUSEKEEPING REQUEST */
		can_check_housekeep();

		/*		TRANSCEIVER COMMUNICATION	*/
		trans_check();
		
		
		/*	REPLY TO MESSAGES FROM MOB4 */
		
		if (send_now == 1)		// Send a reply to the message that was received!
		{		
			for (i = 0; i < 8; i ++)
			{
				send_arr[i] = 0xAB;		// Message to be sent back to the OBC.
			}
			can_send_message(&(send_arr[0]), CAN1_MB7);		//CAN1_MB7 is the command reception MB.
			send_now = 0;
		}
		
		if (send_hk == 1)		// Send a reply to the message that was received!
		{
			for (i = 0; i < 8; i ++)
			{
				send_arr[i] = 0xF0;		// Message to be sent back to the OBC.
			}
			can_send_message(&(send_arr[0]), CAN1_MB6);		//CAN1_MB6 is the HK reception MB.
			send_hk = 0;
		}
		
		if (send_data == 1)		// Send a reply to the message that was received!
		{
			for (i = 0; i < 8; i ++)
			{
				send_arr[i] = 0x00;		// Message to be sent back to the OBC.
			}
			
			//adc_read(&send_arr[0]);	// This line was used to acquire temp from an analog sensor.

			spi_retrieve_temp(&high, &low);
			
			send_arr[1] = high;			// SPI temperature sensor readings.
			send_arr[0] = low;
			
			send_arr[4] = 0x55;			// Temperature indicator.
			
			can_send_message(&(send_arr[0]), CAN1_MB0);		//CAN1_MB0 is the data reception MB.
			send_data = 0;
			
			low = 0;
			high = 0;
		}
		
		if (send_coms == 1)		// Send a reply to the message that was received!
		{
			for (i = 0; i < 8; i ++)
			{
				send_arr[i] = 0x00;		// Message to be sent back to the OBC.
			}
			
			send_arr[0] = trans_msg[0];	// ASCII character which was received.
			
			can_send_message(&(send_arr[0]), CAN1_MB5);		//CAN1_MB0 is the data reception MB.
			send_coms = 0;
		}
	}
}

void sys_init(void) 
{
	// Make sure sys clock is at least 8MHz
	CLKPR = 0x80;  
	CLKPR = 0x00;
	
	io_init();	
	
	timer_init();
	adc_initialize();
	can_init(0);
	can_init_mobs();
	spi_initialize_master();
	
	// Enable global interrupts for Timer execution
	sei();
	
	transceiver_initialize();
	
	SS1_set_high();		// SPI Temp Sensor.
	
	LED_toggle(LED7);
}

void io_init(void) 
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

