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

#define DATA_BUFFER_SIZE 8 // 8 bytes max

static void sys_init(void);
static void io_init(void);

volatile uint8_t CTC_flag;

uint8_t data0[DATA_BUFFER_SIZE];	// Data Buffer for MOb0
uint8_t data1[DATA_BUFFER_SIZE];	// Data Buffer for MOb1
uint8_t data2[DATA_BUFFER_SIZE];	// Data Buffer for MOb2
uint8_t data3[DATA_BUFFER_SIZE];	// Data Buffer for MOb3
uint8_t data4[DATA_BUFFER_SIZE];	// Data Buffer for MOb4
uint8_t data5[DATA_BUFFER_SIZE];	// Data Buffer for MOb5

int main(void)
{		
	// Initialize I/O, Timer, and CAN peripheral
	sys_init();
	
	uint8_t	status, i, mob_number, send_now;
	uint8_t message_arr[8];
	
	// Enable global interrupts for Timer execution
	sei();
	
	// Flash LEDs to indicate program startup
	LED_Reg_Write(0x01);
	delay_ms(250);
	LED_Reg_Write(0x00);
	
    while(1)
    {
		for (i = 0; i < 8; i ++)
		{
			message_arr[i] = 0;			// Reset the message array to zero after each message.
		}
			
		message.pt_data = &data0[0]; // point message object to first element of data buffer
		message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
		message.id.std = SUB0_ID0;  // populate ID field with ID Tag
		message.cmd = CMD_RX_DATA;   // assign this as a receiving message object.
		message.dlc = 8;			 // Max length of a CAN message.
		mob_number = 0;
		
		send_now = 0;
		
		while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb to configure
		
		while(can_get_status(&message, mob_number) == CAN_STATUS_NOT_COMPLETED); // wait for a message to come in.
		
		status = message.status;
		
		if ((status == MOB_RX_COMPLETED) || (status == MOB_RX_COMPLETED_DLCW))
		{
			for (i = 0; i < 8; i ++)
			{
				message_arr[i] = *(message.pt_data + i);
			}
			
			if ((message_arr[0] == 0xFF) && (message_arr[1] == 0xFF) && (message_arr[2] == 0xFF) && (message_arr[3] == 0xFF) 
				&& (message_arr[4] == 0xFF) && (message_arr[5] == 0xFF) && (message_arr[6] == 0xFF) && (message_arr[7] == 0xFF))
			{
				LED_Reg_Write(0x04);	//Toggle LED2 when the appropriate message is received.
				delay_ms(500);
				LED_Reg_Write(0x00);
				send_now = 1;
			}
		}
		
		/*	Message Object 4  */
		
		if (send_now == 1)		// Send a reply to the message that was received!
		{	
			message.pt_data = &data4[0]; // point message object to first element of data buffer
			message.ctrl.ide = 0;		 // standard CAN frame type (2.0A)
			message.id.std = NODE0_ID;  // populate ID field with ID Tag
			message.cmd = CMD_TX_DATA;   // assign this as a transmitting message object.
			message.dlc = 8;			 // Max length of a CAN message.
			mob_number = 4;
		
			for (i = 0; i < 8; i ++)
			{
				data4[i] = 0xAB;		// Message to be sent back to the OBC.
			}
			
			while(can_cmd(&message, mob_number) != CAN_CMD_ACCEPTED); // wait for MOb4 to configure

			while(can_get_status(&message, mob_number) == CAN_STATUS_NOT_COMPLETED); // wait for a message to send or fail.
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

