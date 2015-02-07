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
	*	FILE REFERENCES:	io.h, interrupt, LED.h, Timer.h, can_lib.h
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
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "LED.h"
#include "Timer.h"
#include "can_lib.h"

#define MY_ID_TAG 9

#define DATA_BUFFER_SIZE 2 // 8 bytes max

static void sys_init(void);
static void io_init(void);

volatile uint8_t CTC_flag;

uint8_t response_data[DATA_BUFFER_SIZE];

int main(void)
{		
	// Initialize I/O, Timer, and CAN peripheral
	sys_init();
	
	uint8_t	status, i;
	uint8_t message[8];
	
	// Enable global interrupts for Timer execution
	sei();
	
	// Flash LEDs to indicate program startup
	LED_Reg_Write(0x01);
	delay_ms(250);
	LED_Reg_Write(0x00);

	st_cmd_t reply_message; // response message object
	
    while(1)
    {
		// Simulate collecting local sensor data: put test bytes in response buffer
		//response_data[0] = MY_ID_TAG;
		//if(MY_ID_TAG == 0x80) { response_data[1] = 0x3C; }
		//if(MY_ID_TAG == 0x81) { response_data[1] = 0x0F; }
		for (i = 0; i < 8; i ++)
		{
			message[i] = 0;
		}
			
			
			
			
		reply_message.pt_data = &response_data[0]; // point message object to first element of data buffer
		reply_message.ctrl.ide = 0; // standard CAN frame type (2.0A)
		reply_message.id.std = MY_ID_TAG; // populate ID field with ID Tag
		reply_message.cmd = CMD_RX_DATA; // assign this as a receiving message object.
		reply_message.dlc = 8;			// Max length of a CAN message.
		
		while(can_cmd(&reply_message) != CAN_CMD_ACCEPTED); // wait for MOb to configure
		
		while(can_get_status(&reply_message) == CAN_STATUS_NOT_COMPLETED); // wait for a transmit request to come in.
		
		status = reply_message.status;
		
		if ((status == MOB_RX_COMPLETED) || (status == MOB_RX_COMPLETED_DLCW))
		{
			for (i = 0; i < 4; i ++)
			{
				message[i] = *(reply_message.pt_data + i);
			}
			
			if ((message[0] == 0xFF) && (message[1] == 0xFF) && (message[2] == 0xFF) && (message[3] == 0xFF))
			{
				LED_Reg_Write(0x04);	//Toggle LED2 when the appropriate message is received.
				delay_ms(500);
				LED_Reg_Write(0x00);
			}
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
}

void io_init(void) {
	
	// Init PORTB[7:0] // LED port
	DDRB = 0xFF;
	LED_Reg_Write(0x00); // clear all LEDs
	
	// Init PORTC[7:0] // PORTC[3:2] => RXCAN:TXCAN
	DDRC = 0x00;
	PORTC = 0x00;
	
	// Init PORTD[7:0]
	DDRD = 0x00;
	PORTD = 0x00;
	
	// Init PORTE[2:0]
	DDRE = 0x00;
	PORTD = 0x00;
}