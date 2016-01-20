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
	*	12/05/2015		C:Added transceiver_send(). This function is able to transmit messages from SSM and I managed to
	*					receive it on SmartRF (after changing PKT_CFG0 to 0x00). But there are CRC errors.
	*
	*	01/19/2015		K: I'm revamping all the code here, which includes the initialization function, transceive_send(),
	*					and I'm adding functions for acknowledgements and regular operation of the transceiver.
*/

#include "trans_lib.h"

void transceiver_initialize(void)
{	
	/* SPI is already in MSB first, which is correct for the CC1120. */
	set_CSn(0);
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
	uint8_t state, CHIP_RDYn, rxFirst, rxLast, i, j, txFirst, txLast;
	get_status(&CHIP_RDYn, &state);

	if (millis() - lastCycle < TRANSCEIVER_CYCLE)
	{
		return;
	}
    // We will be in this state if we are waiting for an ACK or we are currently receiving an ACK
	if (tx_mode && (state == STATERX))
	{
		// Waited too long, resend.
		if(millis()-lastTransmit >= ACK_TIMEOUT)
		{
			reg_write2F(TXFIRST, 0x00);
			cmd_str(STX);
			lastTransmit = millis();
		}
	}
// Still sending the data that we have or finished and already received a packet
	else if(tx_mode && (state == STATETX))
	{
		rxFirst = reg_read2F(RXFIRST);
		rxLast = reg_read2F(RXLAST);
		txFirst = reg_read2F(TXFIRST);
		txLast = reg_read2F(TXLAST);
        // Still sending data out, just wait for it
        if (txFirst < txLast){
	        lastTransmit = millis();
        }
		/* Got some data, so there must be a packet waiting for us. */
		else if (rxLast)
		{
			PIN_toggle(LED1);
			uint8_t fifo[128] = {0};
			j = 0;
			rx_length = reg_read(STDFIFO);
            for (uint8_t i = rxFirst; i < rxLast; i++){
	            fifo[j++] = dir_FIFO_read(0x80+i);
            }
			/* Received a packet in TX Mode */
			clear_new_packet();
			for(j = 1; j < fifo[0]; j++)
			{
				new_packet[j - 1] = fifo[j + 1];
			}
			cmd_str(SIDLE);
			cmd_str(SFTX);
			cmd_str(SFRX);
			prepareAck();
			tx_mode = 0;
			rx_mode = 1;
			rx_length = 0;
			cmd_str(SRX);				
		}
		if(rxFirst)
		{
			reg_write2F(RXFIRST, 0);
			reg_write2F(RXLAST, 0);
			lastTransmit = millis();
		}
		//if((millis() - lastTransmit) >= ACK_TIMEOUT)	// Waited too long, resend.
		//{
			//reg_write2F(TXFIRST, 0x00);		// Set TXFIRST to 0
			//cmd_str(STX);					// Put in TX mode.
			//lastTransmit = millis();
		//}
	}

	/* Waiting for some data to come */
	else if (rx_mode && (state == STATERX))
	{
		/* Get the data from the FIFO */
		rxFirst = reg_read2F(RXFIRST);
		rxLast = reg_read2F(RXLAST);
		if(rxFirst || rxLast)
			PIN_toggle(LED1);
		/* Got some data */
		if (rxFirst <= rxLast && rxLast)
		{
            // The first byte that comes when the RX buffer was empty can only be
            // accessed with standard FIFO access. In our case this will be the length
            if (rx_length == 0){
	            rx_length = reg_read(STDFIFO);
	            reg_write2F(RXFIRST, rxFirst);
            }
			uint8_t fifo[128] = {0};
			j = 0;
			for (i = rxFirst; i < rxLast; i++)
			{
				fifo[j++] = dir_FIFO_read(0x80+i);
			}
			/* We have a packet */
			if(fifo[0] <= (rxLast - rxFirst - 1))
			{
				clear_new_packet();
				for(j = 1; j < fifo[0]; j++)
				{
					new_packet[j - 1] = fifo[j + 1];
				}
				packet_receivedf = 1;
				reg_write2F(RXFIRST, rx_length);
				rxFirst += rx_length;
				rx_length = 0;
			}
			/* The packet doesn't seem to be done */
			else if (fifo[0] >= (rxLast - rxFirst - 1)){ }
		}
        // clear RX FIFO if we aren't currently waiting for anything
		if (rxFirst == rxLast && rxFirst && rx_length == 0)
		{
			cmd_str(SIDLE);
			reg_write2F(RXFIRST, 0x00);
			reg_write2F(RXLAST, 0x00);
			cmd_str(SFRX);
			cmd_str(SRX);
			rx_length = 0;		
		}
		reg_write2F(TXFIRST, 0);	// So we can send another ACK.
	}
	else{ }	// Something went wrong.
	
	lastCycle = millis();
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
    reg_write(0x08, 0x0B);            //
    reg_write(0x0C, 0x1C);            //
    reg_write(0x10, 0x00);            //
    reg_write(0x11, 0x04);            //
    reg_write(0x13, 0x05);            //
    reg_write(0x1C, 0xA9);            //
    reg_write(0x1D, 0xCF);            //
    reg_write(0x1E, 0x00);            //
    reg_write(0x20, 0x03);            //
    reg_write2F(0x00, 0x00);          //
    /**************************************/
	
	//modulation and freq deviation settings
	reg_write(DEVIATION_M, 0b01001000);     //DEVIATION_M: 0x48      set DEV_M to 72 which sets freq deviation to 20.019531kHz (with DEV_M=5)
	reg_write(MODCFG_DEV_E, 0b00000101);    //MODCFG_DEV_E: 0x05     set up modulation mode and DEV_E to 5 (see DEV_M register)
	reg_write(FS_CFG, 0b00000100);			//FS_CFG: B00010100      set up LO divider to 8 (410.0 - 480.0 MHz band), out of lock detector disabled
	
	//set preamble
	reg_write(PREAMBLE_CFG1, 0b00001101);         //PREAMBLE_CFG1: 0x00    No preamble
	reg_write_bit(PREAMBLE_CFG0, 5, 1);     //PQT_EN: 0x00           Preamble detection disabled
	
	//TOC_LIMIT
	reg_write_bit2F(TOC_CFG, 7, 0);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	reg_write_bit2F(TOC_CFG, 6, 0);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	
	//set SYNC word
	reg_write_bit(SYNC_CFG1, 6, 0);			//PQT_GATING_EN: 0       PQT gating disabled (preamble not required)
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
	reg_write(PKT_LEN, 0x7F);				//PKT_LEN: 0xFF          set packet max packet length to 0x7F
	reg_write(DEV_ADDR, DEVICE_ADDRESS);	//DEV_ADDR register is set to DEVICE_ADDRESS
	reg_write(RFEND_CFG1, 0b00101110);      //RFEND_CFG1: 0x2E       go to TX after a good packet
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
	//delay_us(1);
	msg = spi_transfer(0x00);
	SS_set_high();
	//delay_ms(1);
	
	return msg;
}

void reg_write(uint8_t addr, uint8_t data)		// Doesn't need to return anything.
{
	uint8_t msg, check, i;
	
	SS_set_low();
	msg = spi_transfer(addr);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	//delay_ms(1);

	return;
}

uint8_t reg_read2F(uint8_t addr)
{
	uint8_t msg;
	msg = 0b10101111;
	
	SS_set_low();
	msg = spi_transfer(msg);
	//delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer(0x00);
	SS_set_high();
	//delay_ms(1);
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
	uint8_t msg, i, check;
	msg = 0b00101111;
	
	SS_set_low();
	spi_transfer(msg);
	//delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	//delay_ms(1);

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
	SS_set_low();
	msg = spi_transfer(addr);
	
	//delay_us(1);
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
	//delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer(0x00);
	SS_set_high();
	//delay_ms(1);
	return msg;
}

void dir_FIFO_write(uint8_t addr, uint8_t data)
{
	cmd_str(SNOP);
	uint8_t msg;
	msg = 0b00111110;
	
	SS_set_low();
	spi_transfer(msg);
	//delay_us(1);
	msg = spi_transfer(addr);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer(data);		// Send the desired data
	SS_set_high();
	//delay_ms(3);
	
	return;
}

/************************************************************************/
/*		set_CSn                                                         */
/*																		*/
/*		This function sets the chip select pin (SS) to either LOW or	*/
/*		HIGH depending on param: state.									*/
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

void trans_check(void)
{
	uint8_t msg = 0, CHIP_RDYn, state;
	
	cmd_str(SRX);
	get_status(&CHIP_RDYn, &state);

	if(state == 0b110 || state == 0b111)
	{
		cmd_str(SIDLE);

		if(SELF_ID != 1)
		{
			PIN_toggle(LED1);
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
	reg_write2F(RXFIRST, 0x00);              //set TX FIRST to 0
	reg_write2F(RXLAST, 0x00); //set TX LAST (maximum OF 0X7F)
	//strobe commands to start TX
	cmd_str(STX);
	tx_mode = 1;
	rx_mode = 0;
	lastTransmit = millis();
}

void transceiver_receive(void)
{
	//cmd_str(SIDLE);
//
	//uint8_t rx_length = 0;
//
	//uint8_t rxFirst = reg_read2F(RXFIRST);
	//uint8_t rxLast = reg_read2F(RXLAST);
	//
	//if(rxFirst || rxLast )
		//PIN_toggle(LED1);
//
	//if (rxFirst < rxLast)
	//{
		////PIN_toggle(LED1);
		//// The first byte that comes when the RX buffer was empty can only be
		//// accessed with standard FIFO access. In our case this will be the length
		////if (rx_length == 0){
		//rx_length = reg_read(STDFIFO);
		////reg_write2F(RXFIRST,rxFirst);
		////}
		////Get the rest of the data
//
		////uint8_t j = 0;
		//for (uint8_t i = rxFirst; i < rxLast; i++)
		//{
			//fifo[i-rxFirst] = dir_FIFO_read(0x80+i);
		//}
	//}
	//// We have a packet
	//if (rx_length <= (rxLast - rxFirst))
	//{
		////for (j = 1; j < rx_length - rxFirst; j++){
		////Serial.print((char) fifo[j]);
		////}
		////Serial.print("\"\n");
		//reg_write2F(RXFIRST, rx_length);
		//rxFirst += rx_length;
		//rx_length = 0;
	//}
	////else if (rx_length >= (rxLast - rxFirst - 1)){
	////}
	////}
	//if (rxFirst == rxLast && rxFirst && rx_length == 0)
	//{
		//cmd_str(SIDLE);
		//reg_write2F(RXFIRST, 0x00);
		//reg_write2F(RXLAST, 0x00);
		//cmd_str(SFRX);
		//cmd_str(SRX);
		//rx_length = 0;
	//}
	//reg_write2F(TXFIRST, 0); // So we can send another ACK
	//cmd_str(SRX);
}

void prepareAck(void)
{
	char* ackMessage = "ACK";
	uint8_t ackAddress = 0xFF, i;
	cmd_str(SIDLE);
	cmd_str(SFTX);
	
	// Reset FIFO registers
	reg_write2F(TXFIRST, 0x00);
	// Put the ACK Packet in the FIFO
	dir_FIFO_write(0, (uint8_t)3 + 2);
	dir_FIFO_write(1, ackAddress);
	
	for(i = 0; i < 3; i++)
		dir_FIFO_write(i+2, ackMessage[i]);
	
	reg_write(TXFIRST, 0);
	reg_write2F(TXLAST, (uint8_t) 3 + 3);
	reg_write2F(RXFIRST, 0x00);
	reg_write2F(RXLAST, 0x00);
	return;
}

void clear_new_packet(void)
{
	for(uint8_t i = 0; i < 128; i ++){
		new_packet[i] = 0;
	}
}
