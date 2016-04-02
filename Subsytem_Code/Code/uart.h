/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		uart_lib.h
	*
	*	PURPOSE:	This is the header file for uart_lib.c
	*
	*	FILE REFERENCES:		
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
	*   http://www.avrfreaks.net/forum/usart-interrupt-atmega32m1
*/
#ifndef UART_H
#define UART_H

#define UART_BUFF_LEN	512
extern volatile uint8_t uart_buffer[UART_BUFF_LEN];
extern volatile uint16_t uart_index;
extern volatile uint8_t uart_overflow;


/* Function Prototypes	*/
void uart_init(void);
uint8_t uart_transmit(uint8_t msg);
uint8_t uart_receive(void);
uint8_t uart_sendmsg(char* msg);
void uart_debug();
void uart_printf(char* format, ...);
void usr_serial_cmd();

#endif