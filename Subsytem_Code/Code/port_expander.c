/*
	Author: Rahman Qureshi
	***********************************************************************
*	FILE NAME:		port_expander.c
*
*	PURPOSE:		API for using the MCP23S17 port expander.
*
*	REFERENCES:		port_expander.h, http://ww1.microchip.com/downloads/en/DeviceDoc/21952b.pdf
*				    https://github.com/n0mjs710
*
*	EXTERNAL VARIABLES:
*
*	EXTERNAL REFERENCES:	Same a File References.
*
*	ABORNOMAL TERMINATION CONDITIONS, ERROR AND WARNING MESSAGES: None yet.
*
*	ASSUMPTIONS, CONSTRAINTS, CONDITIONS:	None
*
*	NOTES:		Testing needs to be done.
*				2016/02/07 Found some errors in the stack. Found that the SPI transmission is out of order 
*							SS is pin 16 which is confirmed to work as an output
*							From Main we had 
*							port_expander_init();							(scope: 02 50 10)
*							port_expander_write(IODIR_BASE, 0b00000000);	(scope: 02 00 00)
*							port_expander_write(GPIO_BASE, 0b11111111);		(scope: 02 48 FF)
*							Current behaviour has the right numbers on scope but in the wrong order. 
*							We tried this same transmission on arduino with success **CONFIG REG needed to be set which was issue before**
*							MISO, SCK
*							X|X|0
*							0|X|0		
*							  ^	
*							  MOSI			
*               2016/03/12 Test and confirmed public API works at least for gpio A. Able to set pin mode, set output state, and read state.
*                          The problem was narrowed down to spi_lib which sent bytes out-of-order and often not at all. A new function, spi_transfer5
*                          was written to fix this.	
*
*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:	
*
*	DEVELOPMENT HISTORY:
*	02/06/2016			Created
*
*	DESCRIPTION:
*
*			The MCP23S17 uses SPI to provide an 16-bit general purpose parallel I/O.
*
*			This file provides methods for setting a pin's mode, reading and writing to it.
*
*/

#include "port_expander.h"
#include "global_var.h"


void port_expander_init()
{
	_write_control_byte = 0b01000000;
	if (A2 == 1) _write_control_byte = _write_control_byte | (0b00001000);
	if (A1 == 1) _write_control_byte = _write_control_byte | (0b00000100);
	if (A0 == 1) _write_control_byte = _write_control_byte | (0b00000010);
	_read_control_byte = _write_control_byte | (0b00000001);
	port_expander_write(IOCON, 0b00001000); // Set configuration register with hardware addressing enabled
}

void port_expander_write(uint8_t register_address, uint8_t data)
{
	PIN_clr(SS_PIN);
	spi_transfer5(_write_control_byte);
	spi_transfer5(register_address);
	spi_transfer5(data);
	PIN_set(SS_PIN);
	return;
}

void port_expander_read(uint8_t register_address, uint8_t* data)
{
	PIN_clr(SS_PIN);
	spi_transfer5(_read_control_byte);
	spi_transfer5(register_address);
	*data = spi_transfer5(0x00); // receive data
	PIN_set(SS_PIN);
	return;
}

void gpioa_pin_mode(uint8_t id, uint8_t io)
{
	uint8_t state;
	port_expander_read(IODIR_BASE, &state);
	if (io == INPUT)
	{
		port_expander_write(IODIR_BASE, state | ( 1 << id));
	}
	else
	{
		port_expander_write(IODIR_BASE, state & ~( 1 << id));
	}
	return;
}


void gpiob_pin_mode(uint8_t id, uint8_t io)
{
	uint8_t state;
	port_expander_read(IODIR_BASE, &state);
	if (io == INPUT)
	{
		port_expander_write(IODIR_BASE + 0x01, state | ( 1 << id)); 
	}
	else
	{
		port_expander_write(IODIR_BASE + 0x01, state & ~( 1 << id));
	}
	return;
}

void set_gpioa_pin(uint8_t id)
{
	uint8_t state;
	port_expander_read(GPIO_BASE, &state);
	port_expander_write(GPIO_BASE , state | ( 1 << id)); // (io & 1) ensures io is 0 or 1
	return;
}

void clr_gpioa_pin(uint8_t id)
{
	uint8_t state;
	port_expander_read(GPIO_BASE, &state);
	port_expander_write(GPIO_BASE , state & ~( 1 << id)); // (io & 1) ensures io is 0 or 1
	return;
}

void set_gpiob_pin(uint8_t id)
{
	uint8_t state;
	port_expander_read(GPIO_BASE + 0x01, &state);
	port_expander_write(GPIO_BASE + 0x01, state | ( 1 << id)); // (io & 1) ensures io is 0 or 1
	return;
}

void clr_gpiob_pin(uint8_t id)
{
	uint8_t state;
	port_expander_read(GPIO_BASE + 0x01, &state);
	port_expander_write(GPIO_BASE + 0x01, state & ~( 1 << id)); // (io & 1) ensures io is 0 or 1
	return;
}

uint8_t read_gpioa_pin(uint8_t id)
{
	uint8_t state;
	port_expander_read(GPIO_BASE, &state);
	if ( (state & (1 << id) ) > 0 )
	{
		return 1;	
	}
	else
	{
		
		return 0;
	}
}

uint8_t read_gpiob_pin(uint8_t id)
{
	uint8_t state;
	port_expander_read(GPIO_BASE + 0x01, &state);
	if ( (state & (1 << id) ) > 0 )
	{
		return 1;
	}
	else
	{
		
		return 0;
	}
}