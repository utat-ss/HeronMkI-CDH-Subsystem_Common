/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		spi_lib.c
	*
	*	PURPOSE:	This program contains functions related to SPI communication
	*				in the ATMEGA32M1.
	*
	*	FILE REFERENCES:
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
	*	2/28/2015		Created.
	*
*/

#include "spi_lib.h"

/************************************************************************/
/*      SPI INITIALIZE                                                  */
/*																		*/
/*		Initialize SPI by setting interrupts off, setting the enable	*/
/*		bit, select the main SPI lines (don't end in _A), setting it as	*/
/*		an SPI slave, communicating with MSB first, clock = MCK / 4.	*/
/*																		*/
/************************************************************************/
void spi_initialize(void)
{
	uint8_t* reg_ptr;
	uint8_t temp = 0;
	
	reg_ptr = MCUCR_BASE;
	temp = 0b10000000;
	*reg_ptr = *reg_ptr | (temp);	// We set SPIPS to 1 (select MISO_A, MOSI_A...)
	
	reg_ptr = SPCR_BASE;
	temp = 0b01110000;
	*reg_ptr = *reg_ptr | (temp);	// Set SPE to 1, MSB first.
	temp = 0b01111100;
	*reg_ptr = *reg_ptr & (temp);	// Turn off SPI interrupt if enabled, set as slave, spiclk = fioclk/4
	
	return;
}


/* Slave Initialize */

//void spi_initialize(void)
//{
	//uint8_t* reg_ptr;
	//uint8_t temp = 0;
	//
	//reg_ptr = MCUCR_BASE;
	//temp = 0b10000000;
	//*reg_ptr = *reg_ptr | (temp);	// We set SPIPS to 1 (select MISO_A, MOSI_A...)
	//
	//reg_ptr = SPCR_BASE;
	//temp = 0b01100000;
	//*reg_ptr = *reg_ptr | (temp);	// Set SPE to 1, MSB first.
	//temp = 0b01101100;
	//*reg_ptr = *reg_ptr & (temp);	// Turn off SPI interrupt if enabled, set as slave, spiclk = fioclk/4
	//
	//return;
//}




/************************************************************************/
/*		SPI TRANSFER                                                    */
/*																		*/
/*		This function takes in a single byte as a parameter and then	*/
/*		proceeds to load this byte into the SPDR register in order to	*/
/*		initiate transmission. It will then loop until the transmission	*/
/*		is completed. If the transmission times out, it returns 0.		*/
/*		A successful transmission corresponds to a return value of 1.	*/
/*																		*/
/************************************************************************/
uint8_t spi_transfer(uint8_t message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	
	reg_ptr = SPDR_BASE;
	*reg_ptr = message;
	
	reg_ptr = SPSR_BASE;
	
	LED_Reg_Write(0x80);					// Toggle LED2 when a character was sent.
	delay_ms(100);
	LED_Reg_Write(0x00);
	delay_ms(25);
	LED_Reg_Write(0x01);
	delay_ms(100);
	LED_Reg_Write(0x00);
	delay_ms(25);

	while(!(*reg_ptr & SPI_SPSR_SPIF))		// Check if the transmission has completed yet.
	{
		if(!timeout--)
			return 0;						// Something went wrong, so the function times out.
	}
	
	return 1;								// Transmission was successful.
}


/************************************************************************/
/*		SPI RECEIVE                                                     */
/*																		*/
/*		This function takes in a pointer which shall be used to store	*/
/*		the incoming byte. This function will wait until a character	*/
/*		is received and the SPIF flag goes high. If the while loop		*/
/*		times out, 0 is returned.1 corresponds to a successful reception*/
/*																		*/
/************************************************************************/
uint8_t spi_receive(uint8_t* r_message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	reg_ptr = SPSR_BASE;
	
	while(!(*reg_ptr & SPI_SPSR_SPIF))		// Wait for a character to received => SPIF goes high.
	{
		if(!timeout--)
		return 0;							// Something went wrong, so the function times out.
	}
	
	reg_ptr = SPDR_BASE;
	*r_message = *reg_ptr;					// Load the received byte into the message pointer.
	
	return 1;								// Reception was successful.
}

void spi_check(void)
{
	uint8_t* reg_ptr;
	uint8_t* receive_char;
	uint8_t	check = 0;
	reg_ptr = SPSR_BASE;
	
	if(*reg_ptr & SPI_SPSR_SPIF)
		check = spi_receive(receive_char);			// A character has been received, and was loaded into the receive pointer.
		
	if(check)
	{
		LED_Reg_Write(0x01);						// Toggle LED0 when a character was received.
		delay_ms(500);
		check = spi_transfer(*receive_char);		// Echo the character back to the master.		
		
		if(check)
		{
			LED_Reg_Write(0x02);					// Toggle LED1 when a character was sent.
			delay_ms(500);
		}
	}
	return;
}

