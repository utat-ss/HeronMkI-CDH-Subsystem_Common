/*
	***********************************************************************
	*	FILE NAME:		uart_lib.c
	*
	*	PURPOSE:	This program contains functions related to UART communication
	*				in the ATMEGA32M1.
	*
	*	FILE REFERENCES:		uart_lib.h
	*
	*	EXTERNAL VARIABLES:		None.
	*
	*	EXTERNAL REFERENCES:	Same a File References.
	*
	*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None.
	*
	*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
	*
	*	NOTES:
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	08/19/2015		Created.
	*
	* http://www.avrfreaks.net/forum/usart-interrupt-atmega32m1
*/
#include "config.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <avr/interrupt.h>
#include "uart.h"

volatile uint8_t uart_buffer[UART_BUFF_LEN] = {0};
volatile uint16_t uart_index = 0;
volatile uint8_t uart_overflow = 0;

#define UART_BAUD	9600

ISR (LIN_TC_vect){
	uart_buffer[uart_index] = LINDAT;
	uart_index++;
	uart_overflow = uart_index >= UART_BUFF_LEN;
}

/************************************************************************/
/* UART INITIALIZE                                                      */
/*																		*/
/* This function initializes the registers which are required for UART	*/
/* communication.														*/
/************************************************************************/

void uart_init(void){
	// initialize chip
	DDRD |= 1<<3;	 // PD3 = TXD is output
	DDRD &= ~(1<<4); // PD4 = RXD is input
	
	// Initialize UART Registers
	LINCR = (1 << LSWRES);                    // Software reset
	LINBRRH = (((F_CPU/UART_BAUD)/16)-1)>>8;  // Baudrate top 8 bits
	LINBRRL = (((F_CPU/UART_BAUD)/16)-1);     // Baudrate lower 8 bits
	LINBTR = (1 << LDISR) | (1 << LBT4);
	LINCR = (1<<LENA)|(1<<LCMD2)|(1<<LCMD1)|(1<<LCMD0); // Turn on UART for full duplex
	LINENIR = 0b00000001;                     // Set the ISR flags for just the receive
	LINSIR = 0b00000001;
	//sei();
}

uint8_t uart_transmit (uint8_t msg){
	uint64_t timeout = F_CPU*30;
	while ((LINSIR & (1 << LBUSY)) && (timeout--)); // Wait while the UART is busy.
	LINDAT = msg;
	return 0;
}

uint8_t uart_sendmsg(char* msg){
	for (int i = 0; i < strlen(msg); i++)
	uart_transmit(msg[i]);
	return 0;
}

uint8_t uart_receive (void){
	uint64_t timeout = F_CPU*30;
	while ((LINSIR & (1 << LBUSY)) && (timeout--)); // Wait while the UART is busy.
	return LINDAT;
}

void uart_printf(char* format, ... ){
	va_list args;
	va_start(args, format);
	char sendBuffer[128] = {0};
	int numWrite = vsnprintf(sendBuffer, 128, format, args);
	va_end(args);
	if (numWrite < 0 || numWrite >= 128){
		uart_sendmsg("Error formatted string too large (uart_printf)\n");
		return;
	}
	uart_sendmsg(sendBuffer);
}

void uart_debug(void){
	uart_printf("UART Debug: index = %d, OVERFLOW = %d\n", uart_index, uart_overflow);
}

void usr_serial_cmd(){
	//inputs command from serial monitor
	if(uart_index){
		int count = 0;
		uint8_t isCommand = 0;
		uint8_t msgLen;
		for(msgLen = 0; msgLen <= uart_index; msgLen++){
			if (uart_buffer[msgLen] == '\n'){
				isCommand = 1;
				break;
			}
		}
		if (isCommand == 0)
		return;
		uart_buffer[msgLen] = 0;
		char cmd[128] = {0};
		char cD1[128] = {0} ;
		char cD2[1] = {0} ;
		// Got the reset command
		if(strcmp("RESET", (char*)uart_buffer) == 0){
			uart_printf("*** Reset the transceiver ***\n");
		}
		else {
			uart_printf("Error: Invalid command\n");
		}
		// Reset the UART Buffer. WARNING: if we get a msg as this happens it will be corrupt
		// memcpy((void*)uart_buffer, (void*)uart_buffer+msgLen+1, UART_BUFF_LEN-(msgLen+1));
		for (count = 0; count < UART_BUFF_LEN - (msgLen + 1); count++){
			uart_buffer[count] = uart_buffer[count+msgLen+1];
			uart_buffer[count+msgLen + 1] = 0;
		}
		uart_index -= msgLen + 1;
	}
}
