/*
	Authors: Keenan Burnett, Louis Pahlavi, Chris Zhang

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
	*					If you're going to mess around with this library, please read the CC1120 User Guide in 
	*					full before you begin.
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
	*					What seems to be happening is that the first byte is being sent but the second is not.
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
	*	12/05/2015		C:Added transceiver_send(). This function is able to transmit messages from SSM and I managed to
	*					receive it on SmartRF (after changing PKT_CFG0 to 0x00). But there are CRC errors.
	*
	*	01/19/2015		I'm revamping all the code here, which includes the initialization function, transceive_send(),
	*					and I'm adding functions for acknowledgments and regular operation of the transceiver.
	*
	*	01/20/2015		Getting rid of functions that we don't really need anymore.
	*
	*	02/04/2016		I was able to send a 76B packet from one SSM to another using the CC1120 tranceivers.
*/

#include "trans_lib.h"

#if (SELF_ID == 0)

static void send_can_value(uint8_t* data);
static void clear_current_tc(void);

void transceiver_initialize(void)
{	
	/* SPI is already in MSB first, which is correct for the CC1120. */
	SS_set_low();
    cmd_str(SRES);		//SRES			reset chip
	delay_ms(100);
    cmd_str(SFRX);		//SFRX          flush RX FIFO
    cmd_str(SFTX);      //SFTX          flush TX FIFO
	/* Settings taken from SmartRF */
	reg_settings();
	/* Calibrate */
	cmd_str(SCAL);                   // Calibrate frequency synthesizer
	delay_ms(250);
	cmd_str(SAFC);					 // Automatic frequency control
	delay_ms(250);

	rx_mode = 1;
	tx_mode = 0;
	rx_length = 0;
	prepareAck();
	/* Put In RX Mode */
	cmd_str(SRX);
	return;	
}

void transceiver_run(void)
{
	uint8_t *state, *CHIP_RDYn, rxFirst, rxLast, check;
	*state = 0;
	*CHIP_RDYn = 0;
	if (millis() - lastCycle < TRANSCEIVER_CYCLE)
		return;
	
	if(tx_mode)
	{
		rx_mode = 0;
		tx_length = reg_read2F(NUM_TXBYTES);
		if(tx_length)
		{
			if(tx_fail_count)
			{
				cmd_str(SIDLE);
				cmd_str(SFTX);
				cmd_str(SRX);
				rx_mode = 1;
				tx_mode = 0;
				tx_fail_count = 0;
				lastCycle = millis();
				return;
			}
			else
			{
				cmd_str(STX);
				tx_fail_count++;
			}
		}
		else
		{
			cmd_str(SRX);
			rx_mode = 1;
			tx_mode = 0;
			lastCycle = millis();
			return;
		}
	}
	if(rx_mode)
	{
		tx_mode = 0;
		rx_length = reg_read2F(NUM_RXBYTES);
		rxFirst = reg_read2F(RXFIRST);
		rxLast = reg_read2F(RXLAST);
		/* Got some data */
		if(rx_length)
		{
			delay_ms(200);		// Relic of working code.
			if(rx_length > REAL_PACKET_LENGTH)
			{
				load_packet();
				/* We have a packet */
				if(new_packet[0] <= (rxLast - rxFirst + 1))		// Length = data + address byte + length byte
				{
					//PIN_toggle(LED1);
					check = store_new_packet();
					rx_length = 0;
					if(!check)									// Packet was accepted and stored internally.
					{
						prepareAck();
						cmd_str(STX);
						return;
					}

				}
			}
			else if(rx_length > ACK_LENGTH)
			{
				load_ack();

				/* We have an acknowledgment */
				if(new_packet[2] == 0x41 && new_packet[3] == 0x43 && new_packet[4] == 0x4B)	// Received proper acknowledgment.
				{
					lastAck = millis();
					lastTransmit = millis();
					//if(last_tx_packet_height)
					current_tm_fullf = 0;				// Second half of packet was sent, set current_tm_fullf to zero.
					//ack_acquired = 1;
				}
			}
			cmd_str(SIDLE);			// Need to get rid of this.
			cmd_str(SFRX);
			cmd_str(SRX);				
		}
		get_status(CHIP_RDYn, state);
		if(*state == 0b110)
		{
			cmd_str(SIDLE);
			cmd_str(SFRX);
			//cmd_str(SRX);
		}
		cmd_str(SRX);			// Make sure we're in RXSTATE while in rx-mode.
	}
	if(millis() - lastAck > ACK_TIMEOUT)
	{
		delay_ms((uint8_t)rand());
		lastAck = millis();
	}
	if(millis() - lastCalibration > CALIBRATION_TIMEOUT)	// Calibrate the transceiver.
	{
		PIN_clr(UHF_RST);
		delay_ms(250);
		PIN_set(UHF_RST);
		transceiver_initialize();
		lastCalibration = millis();
	}
	if(millis() - lastTransmit > TRANSMIT_TIMEOUT)	// Transmit packet (if one is available)
	{
		//PIN_toggle(LED3);
		cmd_str(SIDLE);
		cmd_str(SFRX);
		cmd_str(SFTX);
		delay_ms(5);
		transmit_packet();
		lastTransmit = millis();
	}
	lastCycle = millis();
}

static void send_can_value(uint8_t* data)
{
	send_arr[7] = (SELF_ID << 4)|OBC_ID;
	send_arr[6] = MT_COM;
	send_arr[5] = *(data + 5);
	send_arr[4] = *(data + 4);
	send_arr[3] = *(data + 3);
	send_arr[2] = *(data + 2);
	send_arr[1] = *(data + 1);
	send_arr[0] = *data;
	can_send_message(&(send_arr[0]), CAN1_MB7);
	return;
}

void reg_settings(void)
{
	//high performance settings
	reg_write2F(FS_DIG1, 0x00);				//FS_DIG1: 0x00         Frequency Synthesizer Digital Reg. 1
	reg_write2F(FS_DIG0, 0x5F);				//FS_DIG0: 0x5F         Frequency Synthesizer Digital Reg. 0
	reg_write2F(FS_CAL1, 0x40);				//FS_CAL1: 0x40         Frequency Synthesizer Calibration Reg. 1
	reg_write2F(FS_CAL0, 0x0E);				//FS_CAL0: 0x0E         Frequency Synthesizer Calibration Reg. 0
	reg_write2F(FS_DIVTWO, 0x03);			//FS_DIVTWO: 0x03       Frequency Synthesizer Divide by 2
	reg_write2F(FS_DSM0, 0x33);				//FS_DSM0: 0x33         FS Digital Synthesizer Module Configuration Reg. 0
	reg_write2F(FS_DVC0, 0x17);				//FS_DVCO: 0x17         Frequency Synthesizer Divider Chain Configuration ..
	reg_write2F(FS_PFD, 0x50);				//FS_PFD: 0x50          Frequency Synthesizer Phase Frequency Detector Con..
	reg_write2F(FS_PRE, 0x6E);				//FS_PRE: 0x6E          Frequency Synthesizer Prescaler Configuration
	reg_write2F(FS_REG_DIV_CML, 0x14);      //FS_REG_DIV_CML: 0x14  Frequency Synthesizer Divider Regulator Configurat..
	reg_write2F(FS_SPARE, 0xAC);			//FS_SPARE: 0xAC        Set up Frequency Synthesizer Spare
	reg_write2F(FS_VCO0, 0xB4);				//FS_VCO0: 0xB4         FS Voltage Controlled Oscillator Configuration Reg..
	reg_write2F(XOSC5, 0x0E);				//XOSC5: 0x0E           Crystal Oscillator Configuration Reg. 5
	reg_write2F(XOSC1, 0x03);				//XOSC1: 0x03           Crystal Oscillator Configuration Reg. 0
	
    /************************************/
    //For test purposes only, (2nd block, deleted first one) use values from SmartRF for some bits
    //High performance RX
	cmd_str(SNOP);
    reg_write(SYNC_CFG1, 0x0B);				//
    reg_write(DCFILT_CFG, 0x1C);            //
    reg_write(IQIC, 0x00);					//
    reg_write(CHAN_BW, 0x04);				//
    reg_write(MDMCFG0, 0x05);				//
    reg_write(AGC_CFG1, 0xA9);				//
    reg_write(AGC_CFG0, 0xCF);				//
    reg_write(FIFO_CFG, 0xFF);				//
    reg_write(SETTLING_CFG, 0x03);          //
    reg_write2F(IF_MIX_CFG, 0x00);          //
    /**************************************/
	
	//modulation and freq deviation settings
	reg_write(DEVIATION_M, 0b01001000);     //DEVIATION_M: 0x48      set DEV_M to 72 which sets freq deviation to 20.019531kHz (with DEV_M=5)
	reg_write(MODCFG_DEV_E, 0b00000101);    //MODCFG_DEV_E: 0x05     set up modulation mode and DEV_E to 5 (see DEV_M register)
	reg_write(FS_CFG, 0b00010100);			//FS_CFG: B00010100      set up LO divider to 8 (410.0 - 480.0 MHz band), out of lock detector disabled
	
	//set preamble
	//reg_write(PREAMBLE_CFG1, 0b00001101);         //PREAMBLE_CFG1: 0x00    No preamble
	//reg_write_bit(PREAMBLE_CFG0, 5, 1);     //PQT_EN: 0x00           Preamble detection disabled
	reg_write(PREAMBLE_CFG1, 0x14);
	reg_write(PREAMBLE_CFG0, 0x2A);
	reg_write(SYMBOL_RATE2, 0x73);
	//reg_write(SYMBOL_RATE1, 0x05);
	//reg_write(SYMBOL_RATE0, 0xBC);
	
	//TOC_LIMIT
	reg_write_bit2F(TOC_CFG, 7, 0);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	reg_write_bit2F(TOC_CFG, 6, 0);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	
	//set SYNC word
	reg_write_bit(SYNC_CFG1, 6, 1);			//PQT_GATING_EN: 0       PQT gating disabled (preamble not required)
	reg_write(SYNC_CFG0, 0x17);				//SYNC_CFG0: B00010111   32 bit SYNC word. Bit error qualifier disabled. No check on bit errors
	reg_write(SYNC3, 0x93);					//SYNC3: 0x93            Set SYNC word bits 31:24
	reg_write(SYNC2, 0x0B);					//SYNC2: 0x0B            Set SYNC word bits 23:16
	reg_write(SYNC1, 0x51);					//SYNC1: 0x51            Set SYNC word bits 15:8
	reg_write(SYNC0, 0xDE);					//SYNC0: 0xDE            Set SYNC word bits 7:0
	
	cmd_str(SNOP);
	//set packets
	reg_write_bit(MDMCFG1, 6, 1);			//FIFO_EN: 0             FIFO enable set to true
	reg_write_bit(MDMCFG0, 6, 0);			//TRANSPARENT_MODE_EN: 0 Disable transparent mode
	reg_write(PKT_CFG2, 0b00000000);		//PKT_CFG2: 0x00         set FIFO mode
	reg_write(PKT_CFG1, 0b00110000);		//PKT_CFG1: 0x30         set address check and 0xFF broadcast
	reg_write(PKT_CFG0, 0b00100000);		//PKT_CFG0: 0x30         set variable packet length
	reg_write(PKT_LEN, 0xFF);				//PKT_LEN: 0xFF          set packet max packet length to 0x7F
	reg_write(DEV_ADDR, DEVICE_ADDRESS);	//DEV_ADDR register is set to DEVICE_ADDRESS
	reg_write(RFEND_CFG1, 0b00101110);      //RFEND_CFG1: 0x2E       go to TX after a good packet, RX timeout disabled.
	//reg_write(0x29, 0b00111110);			//RFEND_CFG1: 0x3E       go to RX after a good packet
	reg_write(RFEND_CFG0, 0b00110000);      //RFEND_CFG0: 0x30       go to RX after transmitting a packet
	//reg_write(0x2A, 0b00100000);			//RFEND_CFG0: 0x20       go to TX after transmitting a packet
	
	//set power level
	reg_write(PA_CFG2, 0b01111111);			//PA_CFG2: 0x7F          set POWER_RAMP to 64 (output power to 14.5dBm, equation 21)
	
	//frequency offset setting
	reg_write2F(FREQOFF1, 0);				//FREQOFF1: 0x00         set frequency offset to 0
	reg_write2F(FREQOFF0, 0);				//FREQOFF0: 0x00
	
	//Frequency setting
	reg_write2F(FREQ2, 0x6C);				//FREQ2: 0x6C            set frequency to 434MHz (sets Vco, see equation from FREQ2 section of user guide)
	reg_write2F(FREQ1, 0x80);				//FREQ1: 0x80
	reg_write2F(FREQ0, 0x00);				//FREQ0: 0x00	
	return;
}

uint8_t reg_read(uint8_t addr)
{
	uint8_t addr_new, msg;
	addr_new = addr + 0b10000000;

	SS_set_low();
	msg = spi_transfer(addr_new);		// Send the desired address
	msg = spi_transfer(0x00);
	SS_set_high();
	return msg;
}

void reg_write(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{	
	uint8_t msg = 0;
	SS_set_low();
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	return;
}

uint8_t reg_read2F(uint8_t addr)
{
	uint8_t msg;
	msg = 0b10101111;
	
	SS_set_low();
	msg = spi_transfer(msg);
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(0x00);
	SS_set_high();
	return msg;
}

/************************************************************************/
/*		REG_WRITE2F                                                     */
/*																		*/
/*		This function is used to write a single byte of data to an		*/
/*		address on the CC1120 transceiver (extended address)		    */
/************************************************************************/
void reg_write2F(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{
	cmd_str(SNOP);
	uint8_t msg;
	msg = 0b00101111;
	
	SS_set_low();
	spi_transfer(msg);
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();

	return;
}

/************************************************************************/
/*		GET_STATUS                                                      */
/*																		*/
/*		This function returns the 3 bits which correspond to the status	*/
/*		byte on the CC1120.												*/
/*		Note: 000 = IDLE, 001 = RX, 110 = RX buffer overflow.			*/
/************************************************************************/
void get_status(uint8_t *CHIP_RDYn, uint8_t *state)
{
	uint8_t msg;
	msg = cmd_str(SNOP);
	*CHIP_RDYn = (msg>>7)&1;
	*state = (msg>>4)&7;
	return;
}

/************************************************************************/
/*	CMD_STR                                                             */
/*																		*/
/*	This function is used to send a single command over spi to the		*/
/*	CC1120.																*/
/************************************************************************/
uint8_t cmd_str(uint8_t addr)
{
	uint8_t msg;
	msg = spi_transfer(addr);
	delay_us(1);
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
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(0x00);
	SS_set_high();
	return msg;
}

void dir_FIFO_write(uint8_t addr, uint8_t data)
{
	cmd_str(SNOP);
	uint8_t msg;
	msg = 0b00111110;
	
	SS_set_low();
	spi_transfer(msg);
	msg = spi_transfer(addr);		// Send the desired address
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	
	return;
}

/************************************************************************/
/*		REG_WRITE_BIT                                                   */
/*																		*/
/*		This function is used to write a single bit of data to an		*/
/*		address on the CC1120 transceiver (non-extended address)		*/
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

// Here, address should correspond to the DEVICE_ADDRESS of the transceiver 
// that you want to communicate with.
void transceiver_send(uint8_t* message, uint8_t address, uint8_t length)
{
	uint8_t i;
	cmd_str(SIDLE);
	cmd_str(SFTX);
	// The first byte is the length of the packet (message + 1 for the address)
	dir_FIFO_write(0, length+2);
	// The second byte is the address
	dir_FIFO_write(1, address);
	// The rest is the actual data
	for(i = 0; i < length; i++)
	{
		dir_FIFO_write(i+2, message[i]);
	}
	//set up TX FIFO pointers
	reg_write2F(TXFIRST, 0x00);            //set TX FIRST to 0
	reg_write2F(TXLAST, length+3);				//set TX LAST (maximum OF 0X7F)
	//reg_write2F(RXFIRST, 0x00);              //set TX FIRST to 0
	//reg_write2F(RXLAST, 0x00); //set TX LAST (maximum OF 0X7F)
	//strobe commands to start TX
	cmd_str(STX);
	tx_mode = 1;
	rx_mode = 0;
	lastTransmit = millis();
}

void prepareAck(void)
{
	char* ackMessage = "ACK";
	uint8_t ackAddress = 0xA5, i;
	cmd_str(SIDLE);
	cmd_str(SFTX);
	
	// Reset FIFO registers
	reg_write2F(TXFIRST, 0x00);
	// Put the ACK Packet in the FIFO
	dir_FIFO_write(0, (3 + 2));
	dir_FIFO_write(1, ackAddress);
	
	for(i = 0; i < 3; i++)
		dir_FIFO_write(i+2, ackMessage[i]);
	
	reg_write2F(TXFIRST, 0);
	reg_write2F(TXLAST, (3 + 3));
	reg_write2F(RXFIRST, 0x00);
	reg_write2F(RXLAST, 0x00);
	tx_mode = 1;
	rx_mode = 0;
	lastTransmit = millis();
	return;
}

void clear_new_packet(void)
{
	for(uint8_t i = 0; i < 128; i ++){
		new_packet[i] = 0;
	}
}

uint8_t store_new_packet(void)
{
	uint8_t i;	// packet_height = 0, offset = 0;
	uint16_t pec;
	//uint32_t rsc;
	if(packet_count == 5)		
		return 0xFF;		// Packet_list is currently full, cannot accept new packets.

	/* There is room in the packet list */
	if(new_packet[77] != 0x18)					// Characteristic of B151 in a telecommand.
		return 0xFF;

	packet_count++;
	for (i = 0; i < 76; i++)
	{
		packet_list[packet_count - 1].data[i + 76] = new_packet[i + 2];
	}
	for(i = 0; i < 76; i++)
	{
		packet_list[packet_count - 1].data[i] = 0;
	}
	pec = fletcher16(packet_list[packet_count - 1].data + 2, 150);
	packet_list[packet_count - 1].data[1] = (uint8_t)(pec >> 8);
	packet_list[packet_count - 1].data[0] = (uint8_t)pec;

	//last_rx_packet_height = packet_height;
	return 0x00;
}

// The packet to be transmitted is assumed to be tm_to_downlink[] and be 152 bytes long.
uint8_t transmit_packet(void)
{
	if(!current_tm_fullf)
		return 0xFF;
	
	// Adjust the sequence control variables if an acknowledgment was received.
	//if(ack_acquired)
	//{
		//if(!last_tx_packet_height)
			//last_tx_packet_height = 1;
		//else
			//last_tx_packet_height = 0;
		////transmitting_sequence_control++;
		//ack_acquired = 0;
	//}
	
	// Place the sequence control variables in the packet to be sent.
	//t_message[79] = last_tx_packet_height;
	//t_message[78] = (uint8_t)(transmitting_sequence_control >> 16);
	//t_message[77] = (uint8_t)(transmitting_sequence_control >> 8);
	//t_message[76] = (uint8_t)transmitting_sequence_control;
	//send_can_value(t_message + 74);
	
	//if(last_tx_packet_height)
		//offset = 76;
	tm_to_downlink[151] = 0x18;		// Required in order to quickly authenticate the packet.
	transceiver_send(tm_to_downlink + 76, DEVICE_ADDRESS, 76);
	return 1;
}

void load_packet_to_current_tc(void)
{
	uint8_t i, j;
	if(!packet_count)
	{
		clear_current_tc();
		return;
	}
	for(i = 0; i < PACKET_LENGTH; i++)
	{
		current_tc[i] = packet_list[0].data[i];
	}
	for(j = 0; j < (packet_count - 1); j++)			// Shift the packet list down.
	{
		for(i = 0; i < PACKET_LENGTH; i++)
		{
			packet_list[j].data[i] = packet_list[j + 1].data[i];
		}
	}
	packet_count--;
	return;
}

static void clear_current_tc(void)
{
	uint8_t i;
	for(i = 0; i < PACKET_LENGTH; i++)
	{
		current_tc[i] = 0;
	}
	return;
}

void load_packet(void)
{
	uint8_t i;
	for(i = 0; i < (REAL_PACKET_LENGTH + 2); i++)
	{
		new_packet[i] = reg_read(STDFIFO);
	}
	return;
}

void load_ack(void)
{
	uint8_t i;
	for(i = 0; i < (ACK_LENGTH + 2); i++)
	{
		new_packet[i] = reg_read(STDFIFO);
	}
	return;
}

void setup_fake_tc(void)
{
	uint8_t version, type, sequence_flags, service_type, service_sub_type;
	uint16_t pec;
	version = 0;
	type = 1;
	sequence_flags = 0x02;
	service_type = 3;			// HK Service
	service_sub_type = 9;		// Req HK Definition report
	// Packet Header
	tm_to_downlink[151] = ((version & 0x07) << 5) | ((type & 0x01) << 4) | (0x08);
	tm_to_downlink[150] = HK_TASK_ID;
	tm_to_downlink[149] = sequence_flags;
	tm_to_downlink[148] = transmitting_sequence_control;
	tm_to_downlink[147] = 0x00;
	tm_to_downlink[146] = PACKET_LENGTH - 1;
	version = 1;
	// Data Field Header
	tm_to_downlink[145] = ((version & 0x07) << 4) | 0x8A;
	tm_to_downlink[144] = service_type;
	tm_to_downlink[143] = service_sub_type;
	tm_to_downlink[142] = HK_GROUND_ID;
	tm_to_downlink[140] = 0;
	tm_to_downlink[139] = 0;
	pec = fletcher16(tm_to_downlink + 2, 150);
	tm_to_downlink[1] = (uint8_t)(pec >> 8);
	tm_to_downlink[0] = (uint8_t)(pec);
	
	tm_to_downlink[75] = 0x88;		// Indicator of this being the lower 76 bytes.
	
	return;
}

/************************************************************************/
/* FLETCHER16				                                            */
/* @Purpose: This function runs Fletcher's checksum algorithm on spimem	*/
/* @param: *data: pointer to the point in memory that you would to start*/
/* hashing.																*/
/* @param: count: how many BYTES in memory, you would like to hash		*/
/* @return: the 16-bit checksum value.									*/
/************************************************************************/
uint16_t fletcher16(uint8_t* data, int count)
{
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	int i;
	
	for(i = 0; i < count; i++)
	{
		sum1 = (sum1 + data[i]) % 255;
		sum2 = (sum2 + sum1) % 255;
	}
	return (sum2 << 8) | sum1;
}

#endif