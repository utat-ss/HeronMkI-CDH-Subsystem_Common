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
	*	04/05/2015		I am adding code in order to allow the subsystem micro to communicate
	*					with SPI devices. At this time, I am operating the subsystem micro as a 
	*					master and the OBC as a slave in order to test the subsystem's capabilities
	*					as an SPI master. Later, we will be using the ATMEGA32M1 for relaying 
	*					data from SPI sensors to the OBC.
	*
	*	04/30/2015		The SPI library that I wrote is now operational and is being used to communicate
	*					with the OBC (ATSAM3X8E) as its slave.
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
	// Initialize I/O, Timer, ADC, CAN, and SPI
	sys_init();

	uint8_t	i = 0;
	uint8_t spi_char = 0;
	uint8_t spi_s_message = 0xAA;	// Message to be sent to the slave via SPI.
	
	/* Transceiver testing vars */
	uint8_t state, CHIP_RDYn;
	uint8_t trans_msg = 0;
	
	// Flash LEDs to indicate program startup
	//LED_toggle(LED3);
	//delay_ms(500);
	//LED_toggle(LED3);
	//delay_ms(250);
	//LED_toggle(LED6);
	
	//LED_toggle(LED6);
	//LED_set(LED6);
	//LED_set(LED7);
	
	for (i = 0; i < 8; i ++)
	{
		receive_arr[i] = 0;			// Reset the message array to zero after each message.
	}
			
	send_now = 0;
	send_hk = 0;
	send_data = 0;
	
	uint8_t msg1, msg2;
	
	/*		Begin Main Program Loop					*/	
    while(1)
    {
		
		/* CHECK FOR A GENERAL INCOMING MESSAGE INTO MOB0 */
		//can_check_general();
		
		/* CHECK FOR HOUSEKEEPING REQUEST */
		//can_check_housekeep();
		
		/*		SPI TRANSFER		*/

		//spi_char = spi_transfer(spi_s_message);		// Initiate an SPI transfer with a slave, receive a char into spi_char.
						//
		//if(spi_char == 0xBB)					// Right now, I have the OCB sending back 0xBB as a proof of concept.
		//{				
			//LED_toggle(LED6);					// Toggle LED6 when the correct character was received.
			//delay_ms(125);
			//LED_toggle(LED6);
			//delay_ms(125);
		//}
//
		//spi_char = 0;
		
		/* Testing the COMS Transceiver */
				
	// ****************
		
		//reg_write(0x0A, 0x48);
			//
		////delay_cycles(2);
		//
		//trans_msg = reg_read(0x0A);
	//
		////delay_us(1);
		//
		//if(trans_msg == 0x48)
		//{
		//LED_toggle(LED3);
		//delay_ms(100);
		//LED_toggle(LED3);
		//delay_ms(100);
		//}
		

	
	//cmd_str(SRX);
	
	//reg_write2F(0x0B, 0xAA);             //FREQOFF0: 0x00
	//reg_write2F(0x0A, 0xFF);
	//get_status(&CHIP_RDYn, &state);
	
	//trans_msg = reg_read2F(0x0B);
	//if(trans_msg == 0xAA)
	//{
		//LED_toggle(LED3);
		//delay_ms(100);
		//LED_toggle(LED3);
		//delay_ms(100);
	//}
	//trans_msg = reg_read2F(0x0A);
	//if(trans_msg == 0xFF)
	//{
		//LED_toggle(LED6);
		//delay_ms(100);
		//LED_toggle(LED6);
		//delay_ms(100);
	//}
	//if(state == 0b000)
	//{
		//LED_toggle(LED6);
		//delay_ms(100);
		//LED_toggle(LED6);
		//delay_ms(100);
	//}

/***************/
	//reg_write2F(0x13, 0x5F);          //FS_DIG0: 0x5F         Frequency Synthesizer Digital Reg. 0
	//trans_msg = reg_read2F(0x13);

	get_status(&CHIP_RDYn, &state);
	//if (state == 0b000)
	//{
		//cmd_str(SRX);
		//delay_ms(250);
	//}
	//cmd_str(SRX);
	//delay_ms(250);
  //reg_write2F(0x16, 0x40);          //FS_CAL1: 0x40         Frequency Synthesizer Calibration Reg. 1
  //trans_msg = reg_read2F(0x16);
  //
  //if (trans_msg == 0x40)
  //{
	  //LED_toggle(LED3);
	  //delay_ms(100);
	  //LED_toggle(LED3);
	  //delay_ms(100);
  //}

		
		
	// ********************
		
		//get_status(&CHIP_RDYn, &state);
		//cmd_str(SRX);
		//monitor_LEDs();

		
		if(state == 0b110 || state == 0b111)
		{	
			cmd_str(SIDLE);
			//delay_ms(10);
			LED_toggle(LED3);
			delay_ms(100);
			LED_toggle(LED3);
			delay_ms(100);
			
			// Here we would send our message to the OBC.
			
			cmd_str(SNOP);
			trans_msg = dir_FIFO_read(0x80);
			
			if(trans_msg == 0x0A)
			{
				LED_toggle(LED6);
				delay_ms(100);
				LED_toggle(LED6);
				delay_ms(100);
			}
			
			cmd_str(SFRX);
			//delay_ms(10);
			
			//reg_write2F(0xD2, 0x00);
			//reg_write2F(0xD4, 0xFF);
			
			//if (reg_read2F(0xD2) == 0x00 && reg_read2F(0xD4) == 0xFF)
			//{
				//LED_toggle(LED3);
				//delay_ms(100);
				//LED_toggle(LED3);
				//delay_ms(100);				
			//}
			
			
			//cmd_str(SFTX);
			
			cmd_str(SRX);
			//delay_ms(10);
		}


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
	adc_initialize();
	can_init(0);
	can_init_mobs();
	spi_initialize_master();
	
	// Enable global interrupts for Timer execution
	sei();
	
	transceiver_initialize();
}

void io_init(void) {
	
	// Init PORTB[7:0] // LED port
	DDRB = 0xFE;
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0x01;
	PORTC = 0x00;
	
	// Init PORTD[7:0]
	DDRD = 0x09;		// PD3 is the SS for SPI communications.
	PORTD = 0x09;		// PD3 should only go low during an SPI message.
	
	// Init PORTE[2:0]
	DDRE = 0x00;
	PORTE = 0x00;
}

