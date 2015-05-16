/*
	Author: Keenan Burnett

	***********************************************************************
	*	FILE NAME:		trans_lib.c
	*
	*	PURPOSE:	This program contains functions related to the communications transceiver.
	*
	*	FILE REFERENCES:		trans_lib.h
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
	*	5/05/2015		Created.
	*
*/

#include "trans_lib.h"
#include "Timer.h"
#include "spi_lib.h"

void transceiver_initialize(void)
{
	// SPI is already in MSB first, which is correct for the CC1120.
	
	set_CSn(0);
	
	// Don't need the while loop that was here.
	
	// RESET
	cmd_str(SRES);				// SRES			Reset Chip
	
	cmd_str(SIDLE);
	
	// Reset RX FIFO
	cmd_str(SFRX);				// SRFX			Flush RX FIFO
	
	// Reset TX FIFO
	cmd_str(SFTX);				// STFX			Flush TX FIFO
	
	//********** SETTINGS **********//
	
	//high performance settings
	reg_write2F(0x12, 0x00);          //FS_DIG1: 0x00         Frequency Synthesizer Digital Reg. 1
	reg_write2F(0x13, 0x5F);          //FS_DIG0: 0x5F         Frequency Synthesizer Digital Reg. 0
	reg_write2F(0x16, 0x40);          //FS_CAL1: 0x40         Frequency Synthesizer Calibration Reg. 1
	reg_write2F(0x17, 0x0E);          //FS_CAL0: 0x0E         Frequency Synthesizer Calibration Reg. 0
	reg_write2F(0x19, 0x03);          //FS_DIVTWO: 0x03       Frequency Synthesizer Divide by 2
	reg_write2F(0x1B, 0x33);          //FS_DSM0: 0x33         FS Digital Synthesizer Module Configuration Reg. 0
	reg_write2F(0x1D, 0x17);          //FS_DVCO: 0x17         Frequency Synthesizer Divider Chain Configuration ..
	reg_write2F(0x1F, 0x50);          //FS_PFD: 0x50          Frequency Synthesizer Phase Frequency Detector Con..
	reg_write2F(0x20, 0x6E);          //FS_PRE: 0x6E          Frequency Synthesizer Prescaler Configuration
	reg_write2F(0x21, 0x14);          //FS_REG_DIV_CML: 0x14  Frequency Synthesizer Divider Regulator Configurat..
	reg_write2F(0x22, 0xAC);          //FS_SPARE: 0xAC        Set up Frequency Synthesizer Spare
	reg_write2F(0x27, 0xB4);          //FS_VCO0: 0xB4         FS Voltage Controlled Oscillator Configuration Reg..
	reg_write2F(0x32, 0x0E);          //XOSC5: 0x0E           Crystal Oscillator Configuration Reg. 5
	reg_write2F(0x36, 0x03);          //XOSC1: 0x03           Crystal Oscillator Configuration Reg. 0

	//For test purposes only, use values from SmartRF for all bits
	reg_write2F(0x00, 0x04);          //
	reg_write2F(0x03, 0x00);          //
	reg_write2F(0x09, 0x00);          //
	reg_write2F(0x0F, 0x02);          //
	reg_write2F(0x10, 0xA6);          //
	reg_write2F(0x11, 0x04);          //
	reg_write2F(0x1A, 0x00);          //
	reg_write2F(0x1C, 0xFF);          //
	reg_write2F(0x1E, 0x00);          //
	reg_write2F(0x20, 0x6E);          //
	reg_write2F(0x23, 0x14);          //
	reg_write2F(0x24, 0x00);          //
	reg_write2F(0x28, 0x00);          //
	reg_write2F(0x29, 0x02);          //
	reg_write2F(0x2A, 0x00);          //
	reg_write2F(0x2B, 0x00);          //
	reg_write2F(0x2C, 0x10);          //
	reg_write2F(0x2D, 0x00);          //
	reg_write2F(0x2E, 0x00);          //
	reg_write2F(0x2F, 0x01);          //
	reg_write2F(0x30, 0x01);          //
	reg_write2F(0x31, 0x01);          //
	reg_write2F(0x33, 0xA0);          //
	reg_write2F(0x34, 0x03);          //
	reg_write2F(0x38, 0x00);          //
	reg_write2F(0x39, 0x00);          //
	reg_write2F(0x68, 0x00);          //
	reg_write2F(0x7B, 0x00);          //
	reg_write2F(0x7C, 0x3F);          //
	reg_write2F(0x96, 0x00);          //
	reg_write2F(0x97, 0x00);          //
	reg_write2F(0x98, 0x00);          //
	reg_write2F(0x99, 0x00);          //
	reg_write2F(0x9A, 0x00);          //
	reg_write2F(0x9B, 0x0B);          //
	reg_write2F(0x9C, 0x40);          //
	reg_write2F(0x9D, 0x00);          //
	reg_write2F(0x9E, 0x00);          //
	reg_write2F(0x9F, 0x3C);          //
	reg_write2F(0xA0, 0x00);          //

	//For test purposes only, use values from SmartRF for some bits
	reg_write(0x08, 0x0B);            //*Changed on line 152
	reg_write(0x13, 0x0D);            //
	reg_write(0x26, 0x04);            //*Changed on line 144
	reg_write(0x28, 0x00);            //*Changed on line 145
	reg_write(0x2B, 0x7F);            //
	reg_write2F(0x05, 0x00);          //
	reg_write2F(0x14, 0x00);          //
	reg_write2F(0x26, 0x00);          //
	reg_write2F(0x35, 0x04);          //
	reg_write2F(0x7A, 0xD1);          //
	reg_write2F(0x8D, 0x01);          //

	//High performance RX
	reg_write(0x08, 0x0B);            //
	reg_write(0x0C, 0x1C);            //
	reg_write(0x10, 0x00);            //
	reg_write(0x11, 0x04);            //
	reg_write(0x13, 0x05);            //
	reg_write(0x1C, 0xA9);            //
	reg_write(0x1D, 0xCF);            //
	reg_write(0x1E, 0x00);            //
	reg_write(0x20, 0x03);            //
	reg_write(0x2E, 0x00);            //
	reg_write2F(0x00, 0x00);          //
	
	//modulation and freq deviation settings
	reg_write(0x0A, 0b01001000);       //DEVIATION_M: 0x48      set DEV_M to 72 which sets freq deviation to 20.019531kHz (with DEV_M=5)
	reg_write(0x0B, 0b00000101);       //MODCFG_DEV_E: 0x05     set up modulation mode and DEV_E to 5 (see DEV_M register)
	reg_write(0x21, 0b00000100);       //FS_CFG: B00010100      set up LO divider to 8 (410.0 - 480.0 MHz band), out of lock detector enabled	

	//set preamble
	reg_write(0x0D, 0x00);            //PREAMBLE_CFG1: 0x00    No preamble
	reg_write_bit(0x0E, 5, 0);        //PQT_EN: 0x00           Preamble detection disabled

	//TOC_LIMIT
	reg_write_bit2F(0x02, 7, 0);      //TOC_LIMIT: 0x00        Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	reg_write_bit2F(0x02, 6, 0);      //TOC_LIMIT: 0x00        Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).

	//set SYNC word
	reg_write_bit(0x08, 6, 0);        //PQT_GATING_EN: 0       PQT gating disabled (preamble not required)
	reg_write(0x09, 0b00010111);       //SYNC_CFG0: B00010111   32 bit SYNC word. Bit error qualifier disabled. No check on bit errors
	reg_write(0x04, 0x93);            //SYNC3: 0x93            Set SYNC word bits 31:24
	reg_write(0x05, 0x0B);            //SYNC2: 0x0B            Set SYNC word bits 23:16
	reg_write(0x06, 0x51);            //SYNC1: 0x51            Set SYNC word bits 15:8
	reg_write(0x07, 0xDE);            //SYNC0: 0xDE            Set SYNC word bits 7:0

	//set packets
	reg_write_bit(0x12, 6, 1);        //FIFO_EN: 1             FIFO enable set to true
	reg_write_bit(0x13, 6, 0);        //TRANSPARENT_MODE_EN: 0 Disable transparent mode
	reg_write(0x26, 0x00);            //PKT_CFG2: 0x00         set FIFO mode
	reg_write(0x28, 0x00);            //PKT_CFG0: 0x00         set fixed packet length
	reg_write(0x2E, 0xFF);            //PKT_LEN: 0xFF          set packet length to 0xFF (max)

	//set power level
	reg_write(0x2B, 0b01111111);       //PA_CFG2: 0x7F          set POWER_RAMP to 64 (output power to 14.5dBm, equation 21)

	//frequency offset setting
	reg_write2F(0x0A, 0);             //FREQOFF1: 0x00         set frequency offset to 0
	reg_write2F(0x0B, 0);             //FREQOFF0: 0x00

	//Frequency setting
	reg_write2F(0x0C, 0x6C);          //FREQ2: 0x6C            set frequency to 434MHz (sets Vco, see equation from FREQ2 section of user guide)
	reg_write2F(0x0D, 0x80);          //FREQ1: 0x80
	reg_write2F(0x0E, 0x00);          //FREQ0: 0x00

	//set up GPIO1 to 17
	reg_write(0x00, 17);
	
	//strobe commands to start RX
	cmd_str(SCAL);                   // Calibrate frequency synthesizer

	cmd_str(SAFC);					 // Automatic frequency control

	cmd_str(SRX);                    // Put in RX mode
	
}

/************************************************************************/
/*		set_CSn                                                         */
/*																		*/
/*		This function sets the chip select pin (SS) to either LOW or	*/
/*		HIGH depending on param: state.									*/
/*																		*/
/************************************************************************/

uint8_t reg_read(uint8_t addr)
{
	uint8_t addr_new, msg;
	addr_new = addr + 0b10000000;

	SS_set_low();
	msg = spi_transfer(addr_new);		// Send the desired address
	delay_us(1);
		//if(msg == 0x00)
		//{
			//LED_toggle(LED3);
			//delay_ms(100);
			//LED_toggle(LED3);
			//delay_ms(100);
		//}
	msg = spi_transfer(0x00);
		if(msg == 0xAA)
		{
			LED_toggle(LED3);
			delay_ms(100);
			LED_toggle(LED3);
			delay_ms(100);
		}
	SS_set_high();
	
	return msg;
}

void reg_write(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{
	uint8_t msg;
	
	SS_set_low();
	msg = spi_transfer(addr);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	
	
	return;
}

uint8_t reg_read2F(uint8_t addr)
{
	uint8_t msg;
	msg = 0b1010111;
	
	msg = spi_transfer(msg);		// Address extension command
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(0);			// Read back
	
	return msg;
}

void reg_write2F(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{
	uint8_t msg;
		
	msg = 0b1010111;
	
	msg = spi_transfer(msg);		// Address extension command
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(data);		// Send the desired data
	
	return;
}

void get_status(uint8_t *CHIP_RDYn, uint8_t *state)
{
	uint8_t status_b = cmd_str(SNOP);
	
	*CHIP_RDYn = (status_b / 128) % 2; //7th bit (reading backwards)
	*state = ((status_b / 64) % 2) * 4 + ((status_b / 32) % 2) * 2 + (status_b / 16) % 2; //6th-4th bits (reading backwards)
	return;
}


uint8_t cmd_str(uint8_t addr)
{
	return spi_transfer(addr);
}


uint8_t dir_FIFO_read(uint8_t addr)
{
	uint8_t msg;
	msg = 0b10111110;
		
	msg = spi_transfer(msg);		// Address extension command
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(0);			// Read back
		
	return msg;
}

void dir_FIFO_write(uint8_t addr, uint8_t data)
{
		uint8_t msg;
		
		msg = 0b00111110;
		
		msg = spi_transfer(msg);		// Address extension command
		msg = spi_transfer(addr);		// Send the desired address
		msg = spi_transfer(data);		// Send the desired data
		
		return;
}

void set_CSn(uint8_t state)
{
	if(state)
	SS_set_high();
	else
	SS_set_low();
}

void monitor_LEDs(void)
{
	uint8_t CHIP_RDYn;
	uint8_t state;
	
	//  LED6 goes on if there is a FIFO error
	
	get_status(&CHIP_RDYn, &state);
	
	if (CHIP_RDYn || state == 0b110)
	{
		LED_toggle(LED6);
		delay_ms(100);
		LED_toggle(LED6);
		delay_ms(100);
	}
	//else
		//LED_clr(LED6);
	
	if(state == 0b001)
	{
		LED_toggle(LED7);
		delay_ms(100);
		LED_toggle(LED7);
		delay_ms(100);
	}
	//else
		//LED_clr(LED7);
		
	return;
}

void reg_write_bit(uint8_t reg, uint8_t n, uint8_t data)
{
	uint8_t old_value = reg_read(reg);
	uint8_t	power = 2^n;
	uint8_t	new_value = old_value - (old_value % (2 * power)) + data*power + old_value%power;
	reg_write(reg, new_value);
	return;
}

uint8_t reg_read_bit(uint8_t reg, uint8_t n)
{
	uint8_t old_value = reg_read(reg);
	
	uint8_t power = 2^n;
	uint8_t nth_bit = old_value % (2 * power) / power;
	
	return nth_bit;
}

void reg_write_bit2F(uint8_t reg, uint8_t n, uint8_t data)
{
	uint8_t old_value = reg_read2F(reg);
	uint8_t	power = 2^n;
	uint8_t	new_value = old_value - (old_value % (2 * power)) + data*power + old_value%power;
	reg_write2F(reg, new_value);
	return;
}

uint8_t reg_read_bit2F(uint8_t reg, uint8_t n)
{
	uint8_t old_value = reg_read2F(reg);
	
	uint8_t power = 2^n;
	uint8_t nth_bit = old_value % (2 * power) / power;
	
	return nth_bit;
}

