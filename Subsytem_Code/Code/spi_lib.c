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
	*	03/20/2016		spi_transfer() has been updated.
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
	*reg_ptr = *reg_ptr | (temp);	// Set SPE to 1, MSB first, set as master, CPOL = 0 (SCK low when idle), CPHA = 0
	temp = 0b01010011;
	*reg_ptr = *reg_ptr & (temp);	// Turn off SPI interrupt if enabled, DORD = 0 ==> MSB first, spiclk = fioclk/128	
	return;
}

/************************************************************************/
/*      SPI Transfer 5                                                  */
/*																		*/
/*      spi_transfers0-4 Keenan claims work but they are too flaky.     */
/*      e.g. they will not allow multiple bytes to be sent, send bytes  */
/*      out of order, etc.                                              */
/*                                                                      */
/*      This code definitely works, allowing multiple bytes to be sent  */
/*      and its operation was confirmed on an oscilloscope.             */
/*      This code is used by the port expander                          */
/*																		*/
/************************************************************************/
uint8_t spi_transfer5(uint8_t volatile message)
{
	SPDR = message;
	uint8_t timeout_counter = 0;
	while(!(SPSR & (1<<SPIF)) && timeout_counter++ < 254);
	if (timeout_counter == 254) {
		return 0;
	} else {
		return SPDR; // reading SPSR register and then reading SPDR automatically resets the SPIF bit
	}
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
uint8_t spi_transfer(uint8_t volatile message)
{
	SPDR = message;
	uint8_t timeout_counter = 0;
	while(!(SPSR & (1<<SPIF)) && timeout_counter++ < 254);
	if (timeout_counter == 254) {
		return 0;
		} else {
		return SPDR; // reading SPSR register and then reading SPDR automatically resets the SPIF bit
	}
}

uint8_t spi_transfer2(uint8_t message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	uint8_t receive_char;
	
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

uint8_t spi_transfer4(uint8_t message)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	reg_ptr = SPDR_BASE;
	// Commence the SPI message.
	*reg_ptr = message;
	reg_ptr = SPSR_BASE;
	while(!(*reg_ptr & SPI_SPSR_SPIF))		// Check if the transmission has completed yet.
	{
		if(!timeout--)
		return 0x00;						// Something went wrong, so the function times out.
	}
	return *reg_ptr;
}

// MAX5423 digital potentiometer
void spi_send_shunt_dpot_value(uint8_t message)
{
	uint8_t spi_result, temp;
	uint8_t saved_spi_setting;
	uint8_t change_spi_mode = 0;
	uint8_t* reg_ptr;
	reg_ptr = SPCR_BASE;

	// Disable SPI
	SPCR &= (0b10111111);
	saved_spi_setting = SPCR;
	change_spi_mode = 0b00001100;		// Leave all existing settings as is, but change the mode to CPOL = 1 and CPHA = 1
	// After this we should have SPCR = 0b01011111 which is SPIE = 0 | SPE = 1 | MSB = 0 | MSTR = 1 | CPOL = 1 | CPHA = 1 | 1 | 1 spiclk = fioclk/128
	SPCR |= (change_spi_mode);
	// Enable SPI
	SPCR |= (0b01000000);

	delay_cycles(32);

	SS1_set_low(EPS_DPOT_CS);

	// Set the wiper
	spi_result = spi_transfer(0x00);
	//delay_cycles(8); // This NEEDS to be here! Why? IDK
	spi_result = spi_transfer(message);
	//spi_result = spi_transfer(message);
	SS1_set_high(EPS_DPOT_CS);
	delay_ms(1);
	SS1_set_low(EPS_DPOT_CS);
	// Copy the wiper value to the NV register
	spi_result = spi_transfer(0x20);
	SS1_set_high(EPS_DPOT_CS);
	// Wait for the NV register to FIO its shit
	delay_ms(15);
	
	// Remove our SPI conditions
	// Disable SPI
	SPCR &= (0b10111111);
	*reg_ptr = saved_spi_setting;
	// Enable SPI
	SPCR |= (0b01000000);
	delay_us(1);
	return;
}

void init_temp_sensor(void)
{
	// disable SPI
	// set MISO to OUTPUT
	// set MISO to zero
	// enable SPI
	
	// 
}

uint16_t spi_retrieve_temp(uint8_t chip_select)
{
	uint8_t* reg_ptr;
	uint8_t receive_char;
	uint32_t* temp_raw = malloc(sizeof(uint32_t));
	reg_ptr = SPDR_BASE;
	uint16_t ret_val = 0;
	if(SELF_ID == 0)
		SS1_set_high(COMS_UHF_SS);
		
	/* Disable SPI */
	SPCR &= (0b10111111);
	/* Set MISO as output, level = 0*/
	DDRB |= 0x01;
	PIN_clr(8);
	/* Enable SPI  */
	SPCR |= (0b01000000);
	
	// Commence the SPI message.
	SS1_set_low(chip_select);
	spi_transfer(0);								// Continuous conversion mode.
	spi_transfer(0);
	spi_transfer(0);
	SS1_set_high(chip_select);
	delay_ms(300);
	SS1_set_low(chip_select);
	/* Disable SPI */
	SPCR &= (0b10111111);
	/* Set MISO as input*/
	DDRB &= 0xFE;
	/* Enable SPI  */
	SPCR |= (0b01000000);
	//delay_ms(300);
	*temp_raw = ((uint32_t)spi_transfer(0)) << 8;		// Collect temperature data
	*temp_raw += (uint32_t)spi_transfer(0);
	SS1_set_high(chip_select);
	convert_to_temp(temp_raw);
	ret_val = (uint16_t)*temp_raw;
	if(ret_val < 20)
		ret_val = 20;
	if(ret_val > 35)
		ret_val = 35;	
	
	if(SELF_ID == 0)
		SS1_set_low(COMS_UHF_SS);
	return ret_val;
}

/*******************************IN PROGRESS*************************************/
#if (SELF_ID == 2)
//Acceleration: ADXL362 
//axis: 1=x; 2=y; 3=z
uint16_t spi_retrieve_acc(uint8_t axis)
{	
	uint8_t acc_reg_L, low, high;
	int val;
	uint8_t count = 0;
	int total = 0;
	if (axis == 1)
		acc_reg_L = XDATAL;
	if (axis == 2)
		acc_reg_L = YDATAL;
	if (axis == 3)
		acc_reg_L = ZDATAL;
	for(uint8_t i = 0; i < 10; i++)
	{
		SS1_set_low(PAY_ACCEL_CS);
		spi_transfer(ACC_READ);
		spi_transfer(acc_reg_L);
		low = spi_transfer(0x00);
		high = spi_transfer(0x00);
		SS1_set_high(PAY_ACCEL_CS);
		val = (int)high;
		val = (int)(val << 8);
		val += (int)low;
		if(val < 1200 && val > -1200)
		{
			total += val;
			count++;	
		}
	}
	total /= count;
	return (uint16_t)total;
}

void accelerometer_init(void)
{
	uint8_t msg = 0;
	SS1_set_low(PAY_ACCEL_CS);
	spi_transfer(ACC_WRITE);
	spi_transfer(POWER_CTL);
	msg = spi_transfer(0x02);
	SS1_set_high(PAY_ACCEL_CS);
	uart_sendmsg("ACCELEROMETER INITIALIZED\n\r");
	return msg;
}

// This function is going to be responsible with collecting data from the photodiodes, MIC-style.
// This means absorbence only.
// NOTE: There are currently only 3 MIC chips in the payload.
//void collect_fluorescence_data(void)
//{
	//
//}

void initialize_adc_all(void)
{
	SS1_set_low(ADC1_CS);
	spi_transfer(0x10);		// Set to manual mode, manual channel selection.
	spi_transfer(0x00);
	SS1_set_low(ADC1_CS);
	
	SS1_set_low(ADC2_CS);
	spi_transfer(0x10);		// Set to manual mode, manual channel selection.
	spi_transfer(0x00);
	SS1_set_low(ADC2_CS);

	SS1_set_low(ADC3_CS);
	spi_transfer(0x10);		// Set to manual mode, manual channel selection.
	spi_transfer(0x00);
	SS1_set_low(ADC3_CS);

	SS1_set_low(ADC_FL_CS);
	spi_transfer(0x10);		// Set to manual mode, manual channel selection.
	spi_transfer(0x00);
	SS1_set_low(ADC_FL_CS);

	uart_sendmsg("DIGITAL ADC ALL INITIALIZED\n\r");
	return;
}

uint16_t spi_retrieve_adc_reading(uint8_t adc, uint8_t channel)
{
	uint8_t val1, val2;
	uint16_t ret_val;
	val2 = (channel & 0x01) << 7;	// Channel is split into two different bytes to be sent.
	val1 = channel >> 1;
	if(adc < ADC1_CS && adc > ADC_FL_CS)
		return 0;	// Invalid adc requested.
	
	SS1_set_low(adc);
	spi_transfer(0x10 | val1);
	spi_transfer(val2);
	SS1_set_high(adc);
	delay_ms(100);	// Wait for conversion to complete.
	SS1_set_low(adc);
	val1 = spi_transfer(0x10);
	val2 = spi_transfer(0x00);
	SS1_set_high(adc);
	ret_val = val1;
	ret_val = val1 << 8;
	ret_val += val2;
	return ret_val & 0x0FFF;
}

//Humidity: HIH7000 Series
// Need new port expander before this will work.
void spi_retrieve_humidity(uint8_t *high, uint8_t *low)
{
	
	uint8_t *reg_ptr = SPDR_BASE;
	*reg_ptr = 0;
	SS1_set_low(PAY_HUM_CS);
	delay_us(128);
	high = *reg_ptr;
	delay_us(128);
	low = *reg_ptr;
	SS1_set_high(PAY_HUM_CS);
	return;
}
/**********************Pressure Sensor Driver*****************/

//pressure sensor has 6 x 16-bit calibration values used to calculate
//temperature compensated pressure
//pressure_calibration[0]-[11] used for these values
//6x16-bit calibration values
void pressure_sensor_init(uint8_t* pressure_calibration)
{
	SS1_set_low(PAY_PRESL_CS);
	spi_transfer(PRES_RESET); //reset command for sensor
	delay_ms(3);
	SS1_set_high(PAY_PRESL_CS);
	delay_ms(1);
	for (uint8_t i = 0; i < 12; i+=2)
	{
		SS1_set_low(PAY_PRESL_CS);
		spi_transfer(PROM_READ | ((i / 2 + 1) << 1));		
		pressure_calibration[i + 1] = spi_transfer(0);
		pressure_calibration[i] = spi_transfer(0);
		SS1_set_high(PAY_PRESL_CS);		
		delay_ms(1);
	}
	uart_sendmsg("PRESSURE SENSOR INITIALIZED\n\r");
	return;
}
//Pressure sensor returns two 24-bit values for temp and pressure
//These are stored in array arr in this function. 
//Still need to test the order of the variables etc.

// NOTE: This is uncompensated, need to use calibration values to correct it.
uint32_t spi_retrieve_pressure(void)
{
	//SS1_set_low(PAY_PRESS); //use this
	uint32_t ret_val = 0;
	uint8_t arr[3];
	SS1_set_low(PAY_PRESL_CS);
	spi_transfer(CONVERT_PRES_OSR_4096);
	delay_ms(9);
	SS1_set_high(PAY_PRESL_CS);
	delay_ms(1);
	SS1_set_low(PAY_PRESL_CS);
	spi_transfer(ADC_READ);
	arr[2] = spi_transfer(0);
	arr[1] = spi_transfer(0);
	arr[0] = spi_transfer(0);
	ret_val += (uint32_t)arr[0];
	ret_val += ((uint32_t)arr[1]) << 8;
	ret_val += ((uint32_t)arr[2]) << 16;
	SS1_set_high(PAY_PRESL_CS);
	return ret_val;
}

// NOTE: This is uncompensated, need to use calibration values to correct it.
// This returns the temperature as collected by the pressure sensor.
uint32_t spi_retrieve_pressure_temp(void)
{
	//SS1_set_low(PAY_PRESS); //use this
	uint32_t ret_val = 0;
	uint8_t val = 0;
	SS1_set_low(PAY_PRESL_CS);
	spi_transfer(CONVERT_TEMP_OSR_4096);
	delay_ms(9);
	SS1_set_high(PAY_PRESL_CS);
	delay_ms(1);
	SS1_set_low(PAY_PRESL_CS);
	spi_transfer(ADC_READ);
	for (uint8_t i = 3; i > 0; i--)
	{
		val = spi_transfer(0);
		ret_val += (((uint32_t)val) << (8 * (i - 1)));
	}
	SS1_set_high(PAY_PRESL_CS);
	return ret_val;
}


void convert_temp_press(void)
{
	long int press_raw = 0, temp_raw = 0;
	long int t_ref, temp_sens, tco, off_t1, sens_t1, tcs;
	long int dT, temp, press;
	long int off, sens;	
	
	t_ref = (long int)pressure_calib[8];
	t_ref += ((long int)pressure_calib[9]) << 8;
	dT = temp_raw - t_ref * (1 << 8);
	uart_printf("TREF: %lu\n\r", t_ref);
	uart_printf("DT: %ld\n\r", dT);
			
	temp_sens = (long int)pressure_calib[10];
	temp_sens += ((long int)pressure_calib[11]) << 8;
	temp = 2000 + dT * temp_sens / (1 << 23);
			
	off_t1 = (long int)pressure_calib[2];
	off_t1 += ((long int)pressure_calib[3]) << 8;
	tco = (long int)pressure_calib[6];
	tco += ((long int)pressure_calib[7]) << 8;
	off = (off_t1 * (1 << 17)) + (tco * dT) / (1 << 6);
	uart_printf("OFF: %lu\n\r", off);
			
	sens_t1 = (long int)pressure_calib[0];
	sens_t1 += ((long int)pressure_calib[1]) << 8;
	tcs = (long int)pressure_calib[4];
	tcs += ((long int)pressure_calib[5]) << 8;
	sens = (sens_t1 * (1 << 16)) + (tcs * dT) / (1 << 7);
	uart_printf("SENS: %lu\n\r", sens);
	press = (press_raw * sens / (1 << 21) - off) / (1 << 15);
	return;
}
#endif
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
	uint8_t state;
	switch(sensor_id)
	{
		case COMS_TEMP_SS:
			PIN_set(COMS_TEMP_PIN);
			break;
		case COMS_UHF_SS:
			PIN_set(COMS_UHF_PIN);
			break;
		case COMS_VHF_SS:
			PIN_set(COMS_VHF_PIN);
			break;
		case EPS_TEMP_CS:
			PIN_set(EPS_TEMP_PIN);
			break;
		case EPS_DPOT_CS:
			PIN_set(EPS_DPOT_PIN);
			break;
		case PAY_EXP1_CS:
			PIN_set(32);
			break;
		case PAY_HEATER1_CS:
			set_gpiob_pin(1, 0);
			break;
		case PAY_HEATER2_CS:
			set_gpiob_pin(1, 1);
			break;
		case PAY_HEATER3_CS:
			set_gpiob_pin(1, 2);
			break;
		case PAY_HEATER4_CS:
			set_gpiob_pin(1, 3);
			break;
		case PAY_HEATER5_CS:
			set_gpiob_pin(1, 4);
			break;
		case PAY_VALVE_1A_CS:
			set_gpioa_pin(1, 0);
			break;
		case PAY_VALVE_1B_CS:
			set_gpioa_pin(1, 1);
			break;
		case PAY_VALVE_2A_CS:
			set_gpioa_pin(1, 2);
			break;
		case PAY_VALVE_2B_CS:
			set_gpioa_pin(1, 3);
			break;
		case PAY_VALVE_3A_CS:
			set_gpioa_pin(1, 4);
			break;
		case PAY_VALVE_3B_CS:
			set_gpioa_pin(1, 5);
			break;
		case PAY_VALVE_4A_CS:
			set_gpioa_pin(1, 6);
			break;
		case PAY_VALVE_4B_CS:
			set_gpioa_pin(1, 7);
			break;
		case PAY_PRESL_CS:
			set_gpioa_pin(0, 0);
			//delay_ms(50);
			break;
		case PAY_PRESH_CS:
			set_gpioa_pin(0, 1);
			break;
		case PAY_ACCEL_CS:
			//set_gpioa_pin(0, 2);
			port_expander_write(0, GPIO_BASE, 0x1F);
			//port_expander_read(0, GPIO_BASE, &state);
			//state &= 0x0F;
			//uart_printf("GPIO_BASE: %d\n\r", state);
			//delay_ms(50);
			break;
		case PAY_TEMP_CS:
			//set_gpioa_pin(0, 3);
			port_expander_write(0, GPIO_BASE, 0x1F);
			break;
		case PAY_HUM_CS:
			set_gpioa_pin(0, 4);
			break;
		case ADC1_CS:	// MIC1 -- PE2
			set_gpiob_pin(2, 0);		// Stop comm with ADC on detect board.
			break;
		case ADC2_CS:	// MIC2 -- PE3
			set_gpiob_pin(3, 0);		// Stop comm with ADC on detect board.
			break;
		case ADC3_CS:	// MIC3 -- PE4
			set_gpiob_pin(4, 0);		// Stop comm with ADC on detect board.
			break;
		case ADC_FL_CS:	// FL	-- PE7
			set_gpiob_pin(7, 0);		// Stop comm with ADC on detect board.
			break;
		default:
			break;
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

void SS1_set_low(uint32_t sensor_id)
{
	uint8_t state;
	switch(sensor_id)
	{	
		case COMS_TEMP_SS:
			PIN_clr(COMS_TEMP_PIN);
			break;
		case COMS_UHF_SS:
			PIN_clr(COMS_UHF_PIN);
			break;
		case COMS_VHF_SS:
			PIN_clr(COMS_VHF_PIN);
			break;
		case EPS_TEMP_CS:
			PIN_clr(EPS_TEMP_PIN);
			break;
		case EPS_DPOT_CS:
			PIN_clr(EPS_DPOT_PIN);
			break;
		case PAY_EXP1_CS:
			PIN_clr(32);
			break;
		case PAY_HEATER1_CS:
			clr_gpiob_pin(1, 0);
			break;
		case PAY_HEATER2_CS:
			clr_gpiob_pin(1, 1);
			break;
		case PAY_HEATER3_CS:
			clr_gpiob_pin(1, 2);
			break;
		case PAY_HEATER4_CS:
			clr_gpiob_pin(1, 3);
			break;
		case PAY_HEATER5_CS:
			clr_gpiob_pin(1, 4);
			break;
		case PAY_VALVE_1A_CS:
			clr_gpioa_pin(1, 0);
			break;
		case PAY_VALVE_1B_CS:
			clr_gpioa_pin(1, 1);
			break;
		case PAY_VALVE_2A_CS:
			clr_gpioa_pin(1, 2);
			break;
		case PAY_VALVE_2B_CS:
			clr_gpioa_pin(1, 3);
			break;
		case PAY_VALVE_3A_CS:
			clr_gpioa_pin(1, 4);
			break;
		case PAY_VALVE_3B_CS:
			clr_gpioa_pin(1, 5);
			break;
		case PAY_VALVE_4A_CS:
			clr_gpioa_pin(1, 6);
			break;
		case PAY_VALVE_4B_CS:
			clr_gpioa_pin(1, 7);
			break;
		case PAY_PRESL_CS:
			clr_gpioa_pin(0, 0);
			//delay_ms(50);
			break;		
		case PAY_PRESH_CS:
			clr_gpioa_pin(0, 1);
			break;		
		case PAY_ACCEL_CS:
			//clr_gpioa_pin(0, 2);
			port_expander_write(0, GPIO_BASE, 0x1B);
			//port_expander_read(0, GPIO_BASE, &state);
			//state &= 0x0F;
			//uart_printf("GPIO_BASE: %d\n\r", state);
			//delay_ms(50);
			break;		
		case PAY_TEMP_CS:
			//clr_gpioa_pin(0, 3);
			port_expander_write(0, GPIO_BASE, 0x17);
			break;		
		case PAY_HUM_CS:
			clr_gpioa_pin(0, 4);
			break;
		case ADC1_CS:	// MIC1 -- PE2
			clr_gpiob_pin(2, 0);		// Communicate with PE on detect board.
			clr_gpiob_pin(2, 0);		// Communicate with digital ADC.
			break;
		case ADC2_CS:	// MIC2 -- PE3
			clr_gpiob_pin(3, 0);		// Communicate with PE on detect board.
			clr_gpiob_pin(3, 0);		// Communicate with digital ADC.
			break;
		case ADC3_CS:	// MIC3 -- PE4
			clr_gpiob_pin(4, 0);		// Communicate with PE on detect board.
			clr_gpiob_pin(5, 0);		// Communicate with digital ADC.
			break;
		case ADC_FL_CS:	// FL	-- PE7
			clr_gpiob_pin(7, 0);		// Communicate with PE on detect board.
			break;	
		default:
			break;
	}
}

void SS_set_low(void)
{
	//PORTD &= (0xF7);
	delay_us(1);
}

//set certain ss pin to low
void set_coms_SS_low(uint8_t PIN)
{
	PIN_clr(PIN);
	delay_us(1);
}

//set certain ss pin to high
void set_coms_SS_high(uint8_t PIN)
{
	PIN_set(PIN);
	delay_us(1);
}


