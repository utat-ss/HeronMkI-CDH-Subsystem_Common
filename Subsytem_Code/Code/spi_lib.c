/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		spi_lib.c
	*
	*	PURPOSE:	This program contains functions related to SPI communication
	*				in the ATMEGA32M1.
	*
	*	FILE REFERENCES:		spi_lib.h
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
	*					When reading the SPI documentation for the ATMEGA32M1, note that they 
	*					give signals in terms of !SS as opposed to SS.
	*
	*					This library is incomplete, if the user wants the ATMEGA32M1 to communicate
	*					over SPI as a slave, more code needs to be written.
	*
	*					Pin 2 on the 32M1 corresponds to the SS, and DP10 on the ATSAM3X8E corresponds 
	*					to the Slave-Select for that microcontroller (hence you should connect them).
	*
	*					With SPR0, SPR1 = 11, SPI clock frequency will be set to internal clock/128
	*					which is equal to 62.5 KHz. 
	*
	*	REQUIREMENTS/ FUNCTIONAL SPECIFICATION REFERENCES:
	*	None so far.
	*
	*	DEVELOPMENT HISTORY:
	*	2/28/2015		Created.
	*
	*	4/30/2015		I have edited and fixed several things regarding the SPI_transfer function.
	*
	*					The first thing was that SS needs to be kept low when no communication is 
	*					going on, it will then go high to indicate to the slave that a transfer
	*					is starting.
	*
	*					The second thing is that when an SPI transfer occurs, a byte is sent out by
	*					the master on the MOSI line and simultaneously, a byte is received on the 
	*					MISO line and placed on the SPI receive buffer.
	*
	*					Third, is that you need to wait for the transfer to complete (when SPIF goes high)
	*					before you can read the data register for the received byte. Otherwise, the byte
	*					read will be the one that was just sent on the MOSI line.
	*
	*					I got rid of the 'receive' function because there is no need for it. 
	*
	*	5/04/2015		SPI is now on both the OBC and the SSM and they are communicating well.
	*
	*	5/06/2015		Today I started porting over Louis' code which he uses to allow an Arduino Pro Mini
	*					to communicate with a TI CC1120 transceiver. 
	*
	*					After finishing the initial porting of the code, it didn't work (as expected).
	*
	*					In order to rectify this, I tried simply adding a delay after !SS is set low, didn't work.
	*
	*					I also tried several different combinations of CPOL and CPHA which alter when the bits are
	*					sampled and whether SCK is low/high when idle. Still didn't work, in fact I can't even
	*					read or write to regisers on the transceiver.
	*
	*					I have commented out my first spi_initialize function which was used for communication with
	*					the OBC. The current one is to be used for communication with the CC1120. In the future, these
	*					will be two different initialization functions and I will label them.
	*
	*	12/05/2015		Made some changes to SS_set_low, SS_set_high and spi_transfer.
	*
	*
*/

#include "spi_lib.h"

/************************************************************************/
/*      SPI INITIALIZE MASTER (With OBC)                                */
/*																		*/
/*		Initialize SPI by setting interrupts off, setting the enable	*/
/*		bit, select the main SPI lines (don't end in _A), setting it as	*/
/*		an SPI master, communicating with MSB first, clock = MCK / 4.	*/
/*																		*/
/************************************************************************/
//void spi_initialize_master(void)
//{
	//uint8_t* reg_ptr;
	//uint8_t temp = 0;
	//
	//reg_ptr = MCUCR_BASE;
	//temp = 0b01111111;
	//*reg_ptr = *reg_ptr & (temp);	// We set SPIPS to 0 (select MISO, so NOT MISO_A)
	//
	//reg_ptr = SPCR_BASE;
	//temp = 0b01111111;
	//*reg_ptr = *reg_ptr | (temp);	// Set SPE to 1, MSB first, set as master, spiclk = fioclk/128, CPOL = 1 (SCK high when idle)
	//temp = 0b01011111;
	//*reg_ptr = *reg_ptr & (temp);	// Turn off SPI interrupt if enabled, DORD = 0 ==> MSB first.
	//
	//return;
//}


// SPI Initialize (with transceiver)

/************************************************************************/
void spi_initialize_master(void)
{
	uint8_t* reg_ptr;
	uint8_t temp = 0;
	
	reg_ptr = MCUCR_BASE;
	temp = 0b01111111;
	*reg_ptr = *reg_ptr & (temp);	// We set SPIPS to 0 (select MISO, so NOT MISO_A)
	
	reg_ptr = SPCR_BASE;
	temp = 0b01111111;
	*reg_ptr = *reg_ptr | (temp);	// Set SPE to 1, MSB first, set as master, spiclk = fioclk/128, CPOL = 1 (SCK high when idle), CPHA = 0
	temp = 0b01010011;
	*reg_ptr = *reg_ptr & (temp);	// Turn off SPI interrupt if enabled, DORD = 0 ==> MSB first, spiclk = fioclk/4
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
/*		SPI TRANSFER (as a master)                                      */
/*																		*/
/*		This function takes in a single byte as a parameter and then	*/
/*		proceeds to load this byte into the SPDR register in order to	*/
/*		initiate transmission. It will then loop until the transmission	*/
/*		is completed. If the transmission times out, it returns 0.		*/
/*		A successful transmission will return the byte which was		*/
/*		received on the MISO line during the transfer.					*/
/*																		*/
/************************************************************************/
uint8_t spi_transfer(uint8_t message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	uint8_t receive_char;
	uint8_t i, temp, temp2;
	
	reg_ptr = SPDR_BASE;
	
	// Commence the SPI message.
	//SS_set_low();
	*reg_ptr = message;
		
	reg_ptr = SPSR_BASE;

	while(!(*reg_ptr & SPI_SPSR_SPIF))		// Check if the transmission has completed yet.
	{
		if(!timeout--)
		{
			//SS_set_high();
			return 0x00;						// Something went wrong, so the function times out.
			if(SELF_ID != 1)
			{
				PIN_toggle(LED2);
			}
		}
	}	

	

	
	//SS_set_high();
		
	delay_cycles(11);

	reg_ptr = SPDR_BASE;
	receive_char = *reg_ptr;
	// I was assuming that SPI messages would be received MSB first.
	// Comment out the following if that is not the case.
	
	//temp = 0, temp2 = 0;
	//
	//for (i = 0; i < 8; i ++)
	//{
		//temp2 = receive_char << (7 - i);	// reverses the order of the bits.
		//temp2 = temp2 >> 7;
		//temp2 = temp2 << (7 - i);		
		//temp += temp2;
	//}
	
	return receive_char;					// Transmission was successful, return the character that was received.
}

uint8_t spi_transfer2(uint8_t message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	uint8_t receive_char;
	uint8_t i, temp, temp2;
	
	//cmd_str(SRES);
	
	reg_ptr = SPDR_BASE;
	
	// Commence the SPI message.
	PORTD &= (0xF7);
	//delay_cycles(10);
	*reg_ptr = message;
	//delay_cycles(10);
	reg_ptr = SPSR_BASE;

	while(!(*reg_ptr & SPI_SPSR_SPIF))		// Check if the transmission has completed yet.
	{
		if(!timeout--)
		{
			SS_set_high();
			return 0x00;						// Something went wrong, so the function times out.
		}
	}
	delay_cycles(7);
	SS_set_high();
	
	delay_cycles(10);
		
	reg_ptr = SPDR_BASE;
	receive_char = *reg_ptr;
		
	return receive_char;					// Transmission was successful, return the character that was received.
}


uint8_t spi_transfer3(uint8_t message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	reg_ptr = SPDR_BASE;
	// Commence the SPI message.
	*reg_ptr = message;
	// Delay needed? - Check datasheet.
	reg_ptr = SPSR_BASE;
	while(!(*reg_ptr & SPI_SPSR_SPIF))		// Check if the transmission has completed yet.
	{
		if(!timeout--)
			return 0x00;						// Something went wrong, so the function times out.
	}
	return *reg_ptr;
}


void spi_send_shunt_dpot_value(uint8_t message)
{
	// Set the NV register
	PIN_clr(32);
	spi_transfer3(0x10);
	spi_transfer3(message);
	PIN_set(32);
	
	// Set the wiper
	PIN_clr(32);
	spi_transfer3(0x00);
	spi_transfer3(message);
	PIN_set(32);
	return;
}

void spi_retrieve_temp(uint8_t* high, uint8_t* low)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	uint8_t receive_char;
	uint8_t i, temp, temp2;
	
	reg_ptr = SPDR_BASE;
	
	// Commence the SPI message.

	SS1_set_low(EPS_TEMP);
	*reg_ptr = 0;	// We don't want to pass a message during the first SCK cycles.
	delay_us(128);
	*high = *reg_ptr;
	delay_us(128);
	*low = *reg_ptr;	
	SS1_set_high(EPS_TEMP);
	
	return;
}

/*******************************IN PROGRESS*************************************/

//Just a tentative outline for a function we could implement once all other work
//This doesn't work for acceleration
void spi_read_sensor(uint32_t sensor_name, uint8_t* out_array, uint8_t out_size){
	uint8_t *reg_ptr = SPDR_BASE;
	*reg_ptr = 0;
	int i = 0;
	SS1_set_low(sensor_name);
	delay_us(128);
	for (i=0;i<out_size;i++){
		out_array[i] = *reg_ptr;
		delay_us(128);
	}
}

//Acceleration: ADXL362 
//axis: 1=x; 2=y; 3=z
spi_retrieve_acc(uint8_t *high, uint8_t *low, uint8_t axis){
	
	uint8_t *reg_ptr = SPDR_BASE;
	
	uint8_t acc_reg_H, acc_reg_L;
	if (axis == 1){acc_reg_H = 0x0F; acc_reg_L = 0x0E;}
	if (axis == 2){acc_reg_H = 0x11; acc_reg_L = 0x10;}
	if (axis == 1){acc_reg_H = 0x13; acc_reg_L = 0x12;}
	*reg_ptr = 0x0B;
	SS_set_low();
	delay_us(128);
	*reg_ptr = acc_reg_H;
	delay_us(128);
	*high = *reg_ptr;
	*reg_ptr = acc_reg_L;
	delay_us(128);
	*low = *reg_ptr;
	//check datasheet: is this how burst read works?
	
	SS_set_high();
}

//Humidity: HIH7000 Series

void spi_retrieve_humidity(uint8_t *high, uint8_t *low){
	
	uint8_t *reg_ptr = SPDR_BASE;
	*reg_ptr = 0;
	uint8_t hi, lo;
	
	//SS1_set_low(PAY_HUM);
	SS_set_low();
	
	delay_us(128);
	high = *reg_ptr;
	delay_us(128);
	lo = *reg_ptr;
	
	SS_set_high();
	
}
/**********************Pressure Sensor Driver*****************/

//pressure sensor has 6 x 16-bit calibration values used to calculate
//temperature compensated pressure
void pressure_sensor_init(uint8_t *pressure_calibration){
	//pressure_calibration[0]-[11] used for these values
	//6x16-bit calibration values
	uint8_t* reg_ptr;
	reg_ptr = SPDR_BASE;
	//SS1_set_low(PAY_PRESS); //use this
	SS_set_low(); //use for testing
	*reg_ptr = 0x1E; //reset command for sensor
	delay_us(128); //us or ms??
	int i = 0;
	for (i=0;i<12;i++){
		pressure_calibration[i] = *reg_ptr;
		delay_us(128);}
	SS_set_high();
	}
//Pressure sensor returns two 24-bit values for temp and pressure
//These are stored in array arr in this function. 
//Still need to test the order of the variables etc.

void spi_retrieve_pressure(uint8_t* arr){
	uint8_t* reg_ptr;
	reg_ptr = SPDR_BASE;
	int i = 0;
	
	//SS1_set_low(PAY_HUM); //use this
	SS_set_low(); //use for testing
	*reg_ptr = 0;
	delay_us(128); //us or ms??
	
	
	
	for (i=0; i<6; i++){
		arr[i] = *reg_ptr;
		delay_us(128);
	}
	SS_set_high();	
	};
/************************************************************************/


/************************************************************************/
/*		SS_set_high                                                     */
/*																		*/
/*		This function is to be used to be used to simplify the command	*/
/*		that is required when the SS needs to be set high during an		*/
/*		SPI message.													*/
/*																		*/
/************************************************************************/

void SS_set_high(void) 
{
	//PORTD |= (1 << 3);
	delay_us(1);
}



void SS1_set_high(uint32_t sensor_id)
{
	switch(sensor_id){
		case EPS_TEMP:
			PORTC |= (1 << 4);
	}
}

/************************************************************************/
/*		SS_set_low	                                                    */
/*																		*/
/*		This function is to be used to be used to simplify the command	*/
/*		that is required when the SS needs to be set low during an		*/
/*		SPI message.													*/
/*																		*/
/************************************************************************/

void SS1_set_low(uint32_t sensor_id){
	
	switch(sensor_id){
		
		case EPS_TEMP:
			PORTC &= (0xEF);
	
}
}

void SS_set_low(void)
{
	//PORTD &= (0xF7);
	delay_us(1);
}


/*
void SS1_set_low(void)
{
	PORTC &= (0xEF);
	//delay_us(1);
}
*/


