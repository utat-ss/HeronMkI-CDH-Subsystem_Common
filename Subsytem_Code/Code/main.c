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
	*	FILE REFERENCES:	io.h, interrupt, LED.h, Timer.h, can_lib.h, , adc_lib.h, can_api.h
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
	*	04/05/2015		I am adding code in order to allow the subsystem micro to comminicate
	*					with SPI devices. At this time, I am operating the subsytem micro as a 
	*					master and the OBC as a slave in order to test the subsytem's capabilities
	*					as an SPI master. Later, we will be using the ATMEGA32M1 for relaying 
	*					data from SPI sensors to the OBC.
	*
	*	
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

/* Function Prototypes for functions in this file */
static void sys_init(void);
static void io_init(void);
/**************************************************/

volatile uint8_t CTC_flag;

int main(void)
{		
	// Initialize I/O, Timer, and CAN peripheral
	sys_init();
	
	uint8_t	i = 0;
	uint8_t check = 0;
	uint8_t spi_char = 0;
	uint8_t spi_s_message, spi_r_message = 0;
	uint8_t spi_flag = 0;	// 0 = sending, 1 = receiving.

	spi_s_message = 0xAA;
	
	// Enable global interrupts for Timer execution
	sei();
	
	// Flash LEDs to indicate program startup
	LED_toggle(LED3);
	delay_ms(500);
	LED_toggle(LED3);
	delay_ms(250);
	//LED_toggle(LED6);
	
	for (i = 0; i < 8; i ++)
	{
		receive_arr[i] = 0;			// Reset the message array to zero after each message.
	}
			
	send_now = 0;
	send_hk = 0;
	send_data = 0;	
	
	/*		Initialize CAN Message Objects			*/
	//can_init_mobs();
	
	/*		Initialize SPI communications			*/
	spi_initialize();

	/*		Begin Main Program Loop					*/	
    while(1)
    {
		
		/* CHECK FOR A GENERAL INCOMING MESSAGE INTO MOB0 */
		//can_check_general();
		
		/* CHECK FOR HOUSEKEEPING REQUEST */
		//can_check_housekeep();
		LED_toggle(LED7);
		delay_ms(125);
		LED_toggle(LED7);
		delay_ms(125);
		//
		//if(!spi_flag)
		//{
			spi_char = spi_transfer(spi_s_message);		// Send a character to the slave.
			if(spi_char)
			{
				LED_toggle(LED3);					// Toggle LED3 when a character was sent properly.
				delay_ms(125);
				LED_toggle(LED3);
				delay_ms(125);
				spi_flag = 1;
			}
			
			if(spi_char == 0xFF)
			{				
				LED_toggle(LED6);					// Toggle LED6 when the correct character was received.
				delay_ms(125);
				LED_toggle(LED6);
				delay_ms(125);
			}
			spi_char = 0;
		//}
		
		//if(spi_flag)
		//{
			//check = spi_receive(&spi_r_message);			// A character has been received, and was loaded into the receive pointer.
			//if(check)
			//{
				////LED_toggle(LED6);					// Toggle LED0 when a character was received.
				////delay_ms(125);
				////LED_toggle(LED6);
				////delay_ms(125);
				//if (spi_r_message == spi_s_message)
				//{
					//LED_toggle(LED6);					// Toggle LED0 when a character was received.
					//delay_ms(125);
					//LED_toggle(LED6);
					//delay_ms(125);
				//}
				//spi_flag = 0;
				//spi_r_message = 0;
			//}			
		//}
		/* CHECK FOR AN INCOMING SPI BYTE */

		
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
			
			adc_read(&send_arr[0]);
			send_arr[3] = 0x55;
			
			can_send_message(&(send_arr[0]), CAN1_MB0);		//CAN1_MB0 is the data reception MB.
			send_data = 0;
		}
	}
}

void sys_init(void) {
	// Make sure sys clock is at least 8MHz
	CLKPR = 0x80;  
	CLKPR = 0x00;
	
	io_init();	
	timer_init();
	can_init(0);
	adc_initialize();
}

void io_init(void) {
	
	// Init PORTB[7:0] // LED port
	DDRB = 0xFE;
	//LED_Reg_Write(0x00); // clear all LEDs
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0x01;
	PORTC = 0x00;
	
	// Init PORTD[7:0]
	DDRD = 0x09;		// PD3 is the SS for SPI communicatons.
	PORTD = 0x01;		// PD3 should only go low during an SPI message.
	
	// Init PORTE[2:0]
	DDRE = 0x00;
	PORTD = 0x00;
}

