/*
	Authors: Keenan Burnett, Louis Pahlavi

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
	*	5/06/2015		Today I started working with Louis to port his code over from his Arduino
	*					Pro Mini over to this micro (the ATMEGA32M1). We currently have the ability
	*					to send an ASCII character from one transceiver to another and want to replicate
	*					that capability on this micro.
	*
	*					After completing the initial port with him by my side, it doesn't work.
	*
	*	5/11/2015		Today Louis hooked up our SPI lines to our oscilloscope so that we could see the
	*					bits being transmitted and what the timing looked like.
	*
	*					What seems to be happenning is that the first byte is being sent but the second is not.
	*
	*					We're sending 2 subsequent SPI message and the SS (CSn in the CC1120 documentation) is being
	*					set high and then low.
	*
	*					Sending multiple SPI messages in a row does seem to work with a delay of 1 us.
	*
	*					Discovered an error in reg_read() - I was using addr when I should have been using addr_new.
	*
	*					When using a delay of 1 us, I get a MISO value of 0x0F almost always. 
	*
	*					0x0F is actually the status byte being returned by the CC1120, so that's a good thing. 
	*					Bits 6,5,4 correspond to the status, hence 0x0f means 000 means 'IDLE'.
	*
	*					*CSn needs to be low during the transfer of the two bytes in order for reads & writes to work.
	*					This can be seen in the timing diagrams within the CC1120 User Manual. Google for reference.
	*
	*	5/15/2015		Is the MSB vs. LSB order incorrect? No.
	*
	*					Is the SPI clock speed to high? No.
	*
	*					Note: 100 to 125 ns delays need to be added between consecutive burst access.
	*
	*					125 ns == 1 clock cycle on the SSM.
	*
	*					For some reason, the SS_set_low and SS_set_high commands were commented out.
	*					
	*					CPOL = 1, CPHA = 0, SPR1,0 = 11 is the best combination for communication with
	*					the CC1120.
	*				
	*	5/16/2015		For some reason, the CSn signal is not cycling as I would expect it to on the
	*					oscilloscope.
	*
	*					Switch to a different GPIO pin? Same thing.
	*
	*					Is there a specific range of addresses that I'm outside of accidentally? No.
	*
	*					Changed some settings and added some delays.
	*
	*					Things are starting to sort of work now. Sending FF, getting 7F back.
	*					7F, 3F back and 3F, 1F back. Strange.
	*
	*					This doesn't make a ton of sense, I switched back to what I had before, at least
	*					then I was getting the proper status byte.
	*
	*					Signals are correct on the oscilloscope for read/write, why is the MCU not catching it?
	*
	*					The second byte sent during a read should be zero, it makes the voltages more stable.
	*
	*					I am now sending the correct bytes and receiving the correct bytes from the CC1120.
	*
	*					There are still a few strange issues.
	*
	*					Power levels high enough? Yes.
	*
	*					Restart transceiver every now and then? Unnecessary.
	*
	*					transceiver_initialize seems to make things stop working.
	*
	*					1 us delays between every SPI related function? Nope.
	*
	*					A bunch of write operations in a row? Nope.
	*
	*					SFRX & SFTX are screwing me up now.
	*
	*					I had transceiver_init inside sys_init but I realized it needs to be at the end
	*					because the timer needs to be initialized in order to have delays for SPI.
	*
	*					reg_write2F is now the uncooperative one.
	*
	*					I did screw up setting SS low then high. Fixed it.
	*
	*					The "High Performance RX" section is screwing me over.
	*
	*					I'm not receiving when I should be.
	*
	*					cmd_str was messed up.
	*
	*	05/17/2015		I rewrote get_status such that I reused the code from reg_read.
	*
	*					I am now putting the transceiver into RX successfully!
	*
	*					The next thing I want to see is that I'm getting an RX FIFO error.
	*					This would indicate that I'm receiving bytes and they're filling up
	*					my buffer.
	*
	*					I wasn't returning the correct message in reg_read2F, fixed it.
	*
	*					Do I need the high performance settings? Some of them.
	*
	*					**I am getting rid of all the reg_writes which simply write what
	*					the CC1120 resets them to. (Less problems to deal with)
	*
	*	05/18/2015		We are now probing Louis' code on the oscilloscope for the Arduino Pro
	*					Mini so that we can see what the correct signals look like on the
	*					oscilloscope.
	*
	*					Registers that cause me issues: 0x27, 0x32, 0x36 (extended), 0x0D (regular).
	*
	*	05/19/2015		Error found: I shouldn't be prepending a '1' to the address in reg_read2F.
	*
	*					Error found: Keep CSn high during the transmission of all 3 bytes for reg_read2F.
	*
	*					Funnily enough, just keep CSn low.
	*
	*					With a bit of tinkering with delays, everything works now.
	*
	*	05/21/2015		Adding comments to this code, deleting scaffolding, unnecessary code and test code.
	*
	*	12/05/2015		Added transceiver_send(). This function is able to transmit messages from SSM and I managed to
	*					receive it on SmartRF (after changing PKT_CFG0 to 0x00). But there are CRC errors.
	*		
*/

#include "trans_lib.h"

void transceiver_initialize(void)
{
	uint8_t msg, CHIP_RDY, state;
	
	// SPI is already in MSB first, which is correct for the CC1120.
	
	set_CSn(0);
	
	// Don't need the while loop that was here.
	  
	//RESET 
    cmd_str(SRES);             //SRES                  reset chip
      
    //Reset RX FIFO
    cmd_str(SFRX);             //SFRX                  flush RX FIFO
      
    //Reset TX FIFO
    cmd_str(SFTX);             //SFTX                  flush TX FIFO

	//**************SET UP RX****************//
  
	//high performance settings
	reg_write2F(0x12, 0x00);          //FS_DIG1: 0x00         Frequency Synthesizer Digital Reg. 1
	reg_write2F(0x13, 0x5F);          //FS_DIG0: 0x5F         Frequency Synthesizer Digital Reg. 0
	reg_write2F(0x16, 0x40);          //FS_CAL1: 0x40         Frequency Synthesizer Calibration Reg. 1
	reg_write2F(0x17, 0x0E);          //FS_CAL0: 0x0E         Frequency Synthesizer Calibration Reg. 0
	reg_write2F(0x19, 0x03);          //FS_DIVTWO: 0x03       Frequency Synthesizer Divide by 2
	reg_write2F(0x1B, 0x33);          //FS_DSM0: 0x33         FS Digital Synthesizer Module Configuration Reg. 0
	reg_write2F(0x1D, 0x17);          //FS_DVCO: 0x17         Frequency Synthesizer Divider Chain Configuration ..
	reg_write2F(0x1F, 0x50);          //FS_PFD: 0x50          Frequency Synthesizer Phase Frequency Detector Con..
//  reg_write2F(0x20, 0x6E);          //FS_PRE: 0x6E          Frequency Synthesizer Prescaler Configuration
  
	reg_write2F(0x21, 0x14);          //FS_REG_DIV_CML: 0x14  Frequency Synthesizer Divider Regulator Configurat..
	reg_write2F(0x22, 0xAC);          //FS_SPARE: 0xAC        Set up Frequency Synthesizer Spare
	//reg_write2F(0x27, 0xB4);          //FS_VCO0: 0xB4         FS Voltage Controlled Oscillator Configuration Reg..
	//reg_write2F(0x32, 0x0E);          //XOSC5: 0x0E           Crystal Oscillator Configuration Reg. 5
	//reg_write2F(0x36, 0x03);          //XOSC1: 0x03           Crystal Oscillator Configuration Reg. 0
  
	cmd_str(SNOP);
	//For test purposes only, (2nd block, deleted first one) use values from SmartRF for some bits
	reg_write(0x08, 0x0B);            //*Changed on line 152
	reg_write(0x13, 0x0D);            //
	reg_write(0x26, 0x04);            //*Changed on line 144

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
	reg_write(0x21, 0b00000100);       //FS_CFG: 0x14           set up LO divider to 8 (410.0 - 480.0 MHz band), out of lock detector enabled
			  
	//set preamble
	reg_write(0x0D, 0x00);            //PREAMBLE_CFG1: 0x00    No preamble
	reg_write_bit(0x0E, 5, 0);        //PQT_EN: 0x00           Preamble detection disabled
  
	//TOC_LIMIT
	reg_write_bit2F(0x02, 7, 0);      //TOC_LIMIT: 0x00        Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	reg_write_bit2F(0x02, 6, 0);      //TOC_LIMIT: 0x00        Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
  
	//set SYNC word
	reg_write_bit(0x08, 6, 0);        //PQT_GATING_EN: 0       PQT gating disabled (preamble not required)
	reg_write(0x09, 0x17);            //SYNC_CFG0: 0x17        32 bit SYNC word. Bit error qualifier disabled. No check on bit errors
  	
	//set to fixed packet length
  	reg_write(0x28, 0b00000000);		//PKT_CFG0: 0
	
	//set packets
	reg_write(0x26, 0x00);            //PKT_CFG2: 0x00         set FIFO mode
	reg_write(0x2E, 0x7E);            //PKT_LEN: 0xFF          set packet length to 0xFF (max)  
    
	//Frequency setting
	cmd_str(SNOP);
	reg_write2F(0x0C, 0x6C);          //FREQ2: 0x6C            set frequency to 434MHz (sets Vco, see equation from FREQ2 section of user guide)
	reg_write2F(0x0D, 0x80);          //FREQ1: 0x80

	//reg_write2F(0x0E, 0x00);          //FREQ0: 0x00

	//set up GPIO1 to 17
	//reg_write(0x00, 17);
	
	//strobe commands to start RX
	cmd_str(SCAL);                   // Calibrate frequency synthesizer
	delay_ms(250);
	
	cmd_str(SAFC);					 // Automatic frequency control
	delay_ms(250);
	
	//reg_write2F(0xD2, 0x00);
	//reg_write2F(0xD4, 0xFF);
	
	msg_received = 0;

	cmd_str(SRX);                    // Put in RX mode
	
}

/************************************************************************/
/*		REG_WRITE2F                                                     */
/*																		*/
/*		This function is used to read a single byte of data from an		*/
/*		address on the CC1120 transceiver (extended address)		    */
/*																		*/
/************************************************************************/

uint8_t reg_read(uint8_t addr)
{
	uint8_t addr_new, msg;
	addr_new = addr + 0b10000000;

	SS_set_low();
	msg = spi_transfer(addr_new);		// Send the desired address
	delay_us(1);
	msg = spi_transfer(0x00);

	SS_set_high();
	
	delay_ms(1);
	
	return msg;
}

/************************************************************************/
/*		REG_WRITE                                                       */
/*																		*/
/*		This function is used to write a single byte of data to an		*/
/*		address on the CC1120 transceiver (non-extended address)		*/
/*																		*/
/************************************************************************/

void reg_write(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{
	uint8_t msg, check, i;
	
	SS_set_low();
	msg = spi_transfer(addr);		// Send the desired address
	delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	
	delay_ms(1);

	return;
}

/************************************************************************/
/*		REG_WRITE2F                                                     */
/*																		*/
/*		This function is used to read a single byte of data from an		*/
/*		address on the CC1120 transceiver (extended address)		    */
/*																		*/
/************************************************************************/

uint8_t reg_read2F(uint8_t addr)
{
	uint8_t msg;
	msg = 0b10101111;
	
	SS_set_low();
	msg = spi_transfer(msg);
	delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	delay_us(1);
	msg = spi_transfer(0x00);
	SS_set_high();
	delay_ms(1);
	return msg;
}

/************************************************************************/
/*		REG_WRITE2F                                                     */
/*																		*/
/*		This function is used to write a single byte of data to an		*/
/*		address on the CC1120 transceiver (extended address)		    */
/*																		*/
/************************************************************************/

void reg_write2F(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{
	cmd_str(SNOP);
	uint8_t msg, i, check;
	msg = 0b00101111;
	
	SS_set_low();
	spi_transfer(msg);
	delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	delay_ms(1);

	return;
}

/************************************************************************/
/*		GET_STATUS                                                      */
/*																		*/
/*		This function returns the 3 bits which correspond to the status	*/
/*		byte on the CC1120.												*/
/*		Note: 000 = IDLE, 001 = RX, 110 = RX buffer overflow.			*/
/*																		*/
/************************************************************************/

void get_status(uint8_t *CHIP_RDYn, uint8_t *state)
{
	uint8_t msg;

	msg = cmd_str(SNOP);
	
	msg = msg & ~0x80;
	msg = msg >> 4;
	*state = msg;
	
	return;
}

/************************************************************************/
/*	CMD_STR                                                             */
/*																		*/
/*	This function is used to send a single command over spi to the		*/
/*	CC1120.																*/
/*																		*/
/************************************************************************/

uint8_t cmd_str(uint8_t addr)
{
	uint8_t msg;
	SS_set_low();
	msg = spi_transfer(addr);
	
	delay_us(1);
	SS_set_high();
	return msg;
}

/************************************************************************/
/*	DIR_FIFO_READ                                                       */
/*																		*/
/*	This function takes in an address which corresponds to somewhere	*/
/*	on the CC1120's FIFO and return the byte which was located there.	*/
/*																		*/
/************************************************************************/

uint8_t dir_FIFO_read(uint8_t addr)
{
	cmd_str(SNOP);
	uint8_t msg;
	msg = 0b10111110;
	
	SS_set_low();
	msg = spi_transfer(msg);
	delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	delay_us(1);
	msg = spi_transfer(0x00);
	SS_set_high();
	delay_ms(1);
	return msg;
}

/************************************************************************/
/*	DIR_FIFO_READ                                                       */
/*																		*/
/*	This function takes in an address which corresponds to somewhere	*/
/*	on the CC1120's FIFO and a byte of data which is written to the FIFO*/
/*																		*/
/************************************************************************/

void dir_FIFO_write(uint8_t addr, uint8_t data)
{
	cmd_str(SNOP);
	uint8_t msg;
	msg = 0b00111110;
	
	SS_set_low();
	spi_transfer(msg);
	delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	delay_ms(1);
	
	return;
}

/************************************************************************/
/*		set_CSn                                                         */
/*																		*/
/*		This function sets the chip select pin (SS) to either LOW or	*/
/*		HIGH depending on param: state.									*/
/*																		*/
/************************************************************************/

void set_CSn(uint8_t state)
{
	if(state)
		SS_set_high();
	else
		SS_set_low();
}

/************************************************************************/
/*		REG_WRITE_BIT                                                   */
/*																		*/
/*		This function is used to write a single bit of data to an		*/
/*		address on the CC1120 transceiver (non-extended address)		*/
/*																		*/
/************************************************************************/

void reg_write_bit(uint8_t reg, uint8_t n, uint8_t data)
{
	uint8_t msg, temp;
	msg = reg_read(reg);
	if(!data)
	{
		temp = ~(1 << n);
		msg = temp & msg;
	}
	else
	{
		temp = 1 << n;
		msg = temp | msg;
	}
	reg_write(reg, msg);
	return;
}

/************************************************************************/
/*		REG_WRITE_BIT                                                   */
/*																		*/
/*		This function is used to write a single bit of data to an		*/
/*		address on the CC1120 transceiver (extended address)			*/
/*																		*/
/************************************************************************/

void reg_write_bit2F(uint8_t reg, uint8_t n, uint8_t data)
{
	uint8_t msg, temp;
	msg = reg_read2F(reg);
	if(!data)
	{
		temp = ~(1 << n);
		msg = temp & msg;
	}
	else
	{
		temp = 1 << n;
		msg = temp | msg;
	}
	reg_write2F(reg, msg);
	return;
}

void trans_check(void)
{
	uint8_t msg = 0, CHIP_RDYn, state;
	
	get_status(&CHIP_RDYn, &state);

	if(state == 0b110 || state == 0b111)
	{
		cmd_str(SIDLE);

		if(SELF_ID != 1)
		{
			PIN_toggle(LED3);
		}
		
		// Here we would send our message to the OBC.
		
		cmd_str(SNOP);
		trans_msg[0] = dir_FIFO_read(0x80);
		trans_msg[1] = dir_FIFO_read(0x81);
		trans_msg[2] = dir_FIFO_read(0x82);
		trans_msg[3] = dir_FIFO_read(0x83);
		trans_msg[4] = dir_FIFO_read(0x84);
		trans_msg[5] = dir_FIFO_read(0x85);
		
		//if(msg == 0x0A)
		//{
			//LED_toggle(LED6);
			//delay_ms(100);
			//LED_toggle(LED6);
			//delay_ms(100);
		//}
		msg_received = 1;		// Indicates that in main() we will send trans_msg to the OBC via CAN.
		
		cmd_str(SFRX);
		
		//reg_write2F(0xD2, 0x00);
		//reg_write2F(0xD4, 0xFF);
		
		//cmd_str(SFTX);
		
		cmd_str(SRX);
	}
	return;
}
uint8_t flag=0;

void transceiver_send(){
	// Set it to IDLE and flush the TX buffer before continuing to send data
	cmd_str(SIDLE);
	cmd_str(SFTX);
	uint8_t message[10]={0x32,0x56,0x68,0x06,0x09,0x05,0x04,0x22,0x03,0x66};
	// The first byte is the length of the packet (message + 1 for the address)
	//dir_FIFO_write(0,12);
	// The second byte is the address
	//dir_FIFO_write(1,0x00);
	// The rest is the actual data
	for(int i=0; i<10; i++)
		dir_FIFO_write(i, message[i]);
	//set up TX FIFO pointers
	reg_write2F(TXFIRST, 0x00);            //set TX FIRST to 0
	reg_write2F(TXLAST, 0x7E); //set TX LAST (maximum OF 0X7F)
	reg_write2F(RXFIRST, 0x00);              //set TX FIRST to 0
	reg_write2F(RXLAST, 0x00); //set TX LAST (maximum OF 0X7F)
	//strobe commands to start TX
	cmd_str(STX);
}