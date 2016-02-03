/*
 * comm_control.c
 *
 * Created: 2016/1/30 16:15:23
 *  Author: Chris Zhang
 */ 
#include "comm_comtrol.h"
#include "trans_lib.h"

void transceiver_init(uint8_t tsvNumber, bool isBeacon)
{
	/* SPI is already in MSB first, which is correct for the CC1120. */
	set_SS_low(tsvNumber);
	cmd_str_to_transceiver(SRES,tsvNumber);		//SRES			reset chip
	delay_ms(100);
	cmd_str_to_transceiver(SFRX,tsvNumber);		//SFRX          flush RX FIFO
	cmd_str_to_transceiver(SFTX,tsvNumber);      //SFTX          flush TX FIFO
	/* Settings taken from SmartRF */
	if(tsvNumber==VHFTSV)
		reg_settings_VHF();
	else if(tsvNumber==UHFTSV && isBeacon)
		reg_settings_UHF_Beacon();
	else if(tsvNumber==UHFTSV)
		reg_settings_UHF();
		
	/* Calibrate */
	cmd_str_to_transceiver(SCAL,tsvNumber);                   // Calibrate frequency synthesizer
	delay_ms(250);
	cmd_str_to_transceiver(SAFC,tsvNumber);					 // Automatic frequency control
	delay_ms(250);

	rx_mode = 1;
	tx_mode = 0;
	rx_length = 0;
	prepareAck();
	/* Put In RX Mode */
	cmd_str_to_transceiver(SRX,tsvNumber);
	return;
}

//This was taken from 2016.1.30 Commit:4856111
void reg_settings_UHF(void)
{
	//high performance settings
	reg_write2F_to_tsv(FS_DIG1, 0x00,UHFTSV);				//FS_DIG1: 0x00         Frequency Synthesizer Digital Reg. 1
	reg_write2F_to_tsv(FS_DIG0, 0x5F,UHFTSV);				//FS_DIG0: 0x5F         Frequency Synthesizer Digital Reg. 0
	reg_write2F_to_tsv(FS_CAL1, 0x40,UHFTSV);				//FS_CAL1: 0x40         Frequency Synthesizer Calibration Reg. 1
	reg_write2F_to_tsv(FS_CAL0, 0x0E,UHFTSV);				//FS_CAL0: 0x0E         Frequency Synthesizer Calibration Reg. 0
	reg_write2F_to_tsv(FS_DIVTWO, 0x03,UHFTSV);			//FS_DIVTWO: 0x03       Frequency Synthesizer Divide by 2
	reg_write2F_to_tsv(FS_DSM0, 0x33,UHFTSV);				//FS_DSM0: 0x33         FS Digital Synthesizer Module Configuration Reg. 0
	reg_write2F_to_tsv(FS_DVC0, 0x17,UHFTSV);				//FS_DVCO: 0x17         Frequency Synthesizer Divider Chain Configuration ..
	reg_write2F_to_tsv(FS_PFD, 0x50,UHFTSV);				//FS_PFD: 0x50          Frequency Synthesizer Phase Frequency Detector Con..
	reg_write2F_to_tsv(FS_PRE, 0x6E,UHFTSV);				//FS_PRE: 0x6E          Frequency Synthesizer Prescaler Configuration
	reg_write2F_to_tsv(FS_REG_DIV_CML, 0x14,UHFTSV);      //FS_REG_DIV_CML: 0x14  Frequency Synthesizer Divider Regulator Configurat..
	reg_write2F_to_tsv(FS_SPARE, 0xAC,UHFTSV);			//FS_SPARE: 0xAC        Set up Frequency Synthesizer Spare
	reg_write2F_to_tsv(FS_VCO0, 0xB4,UHFTSV);				//FS_VCO0: 0xB4         FS Voltage Controlled Oscillator Configuration Reg..
	reg_write2F_to_tsv(XOSC5, 0x0E,UHFTSV);				//XOSC5: 0x0E           Crystal Oscillator Configuration Reg. 5
	reg_write2F_to_tsv(XOSC1, 0x03,UHFTSV);				//XOSC1: 0x03           Crystal Oscillator Configuration Reg. 0
	
	/************************************/
	//For test purposes only, (2nd block, deleted first one) use values from SmartRF for some bits
	//High performance RX
	reg_write_to_tsv(SYNC_CFG1, 0x0B,UHFTSV);				//
	reg_write_to_tsv(DCFILT_CFG, 0x1C,UHFTSV);            //
	reg_write_to_tsv(IQIC, 0x00,UHFTSV);					//
	reg_write_to_tsv(CHAN_BW, 0x04,UHFTSV);				//
	reg_write_to_tsv(MDMCFG0, 0x05,UHFTSV);				//
	reg_write_to_tsv(AGC_CFG1, 0xA9,UHFTSV);				//
	reg_write_to_tsv(AGC_CFG0, 0xCF,UHFTSV);				//
	reg_write_to_tsv(FIFO_CFG, 0x00,UHFTSV);				//
	reg_write_to_tsv(SETTLING_CFG, 0x03,UHFTSV);          //
	reg_write2F_to_tsv(IF_MIX_CFG, 0x00,UHFTSV);          //
	/**************************************/
	
	//modulation and freq deviation settings
	reg_write_to_tsv(DEVIATION_M, 0b01001000,UHFTSV);     //DEVIATION_M: 0x48      set DEV_M to 72 which sets freq deviation to 20.019531kHz (with DEV_M=5)
	reg_write_to_tsv(MODCFG_DEV_E, 0b00000101,UHFTSV);    //MODCFG_DEV_E: 0x05     set up modulation mode and DEV_E to 5 (see DEV_M register)
	reg_write_to_tsv(FS_CFG, 0b00000100,UHFTSV);			//FS_CFG: B00010100      set up LO divider to 8 (410.0 - 480.0 MHz band), out of lock detector disabled
	
	//set preamble
	reg_write_to_tsv(PREAMBLE_CFG1, 0b00001101,UHFTSV);         //PREAMBLE_CFG1: 0x00    No preamble
	reg_write_bit_to_tsv(PREAMBLE_CFG0, 5, 1,UHFTSV);     //PQT_EN: 0x00           Preamble detection disabled
	
	//TOC_LIMIT
	reg_write_bit2F_to_tsv(TOC_CFG, 7, 0,UHFTSV);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	reg_write_bit2F_to_tsv(TOC_CFG, 6, 0,UHFTSV);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	
	//set SYNC word
	reg_write_bit_to_tsv(SYNC_CFG1, 6, 0,UHFTSV);			//PQT_GATING_EN: 0       PQT gating disabled (preamble not required)
	reg_write_to_tsv(SYNC_CFG0, 0x17,UHFTSV);				//SYNC_CFG0: B00010111   32 bit SYNC word. Bit error qualifier disabled. No check on bit errors
	reg_write_to_tsv(SYNC3, 0x93,UHFTSV);					//SYNC3: 0x93            Set SYNC word bits 31:24
	reg_write_to_tsv(SYNC2, 0x0B,UHFTSV);					//SYNC2: 0x0B            Set SYNC word bits 23:16
	reg_write_to_tsv(SYNC1, 0x51,UHFTSV);					//SYNC1: 0x51            Set SYNC word bits 15:8
	reg_write_to_tsv(SYNC0, 0xDE,UHFTSV);					//SYNC0: 0xDE            Set SYNC word bits 7:0
	
	cmd_str_to_transceiver(SNOP,UHFTSV);
	//set packets
	reg_write_bit_to_tsv(MDMCFG1, 6, 1,UHFTSV);			//FIFO_EN: 0             FIFO enable set to true
	reg_write_bit_to_tsv(MDMCFG0, 6, 0,UHFTSV);			//TRANSPARENT_MODE_EN: 0 Disable transparent mode
	reg_write_to_tsv(PKT_CFG2, 0b00000000,UHFTSV);		//PKT_CFG2: 0x00         set FIFO mode
	reg_write_to_tsv(PKT_CFG1, 0b00110000,UHFTSV);		//PKT_CFG1: 0x30         set address check and 0xFF broadcast
	reg_write_to_tsv(PKT_CFG0, 0b00100000,UHFTSV);		//PKT_CFG0: 0x30         set variable packet length
	reg_write_to_tsv(PKT_LEN, 0x7F,UHFTSV);				//PKT_LEN: 0xFF          set packet max packet length to 0x7F
	reg_write_to_tsv(DEV_ADDR, DEVICE_ADDRESS,UHFTSV);	//DEV_ADDR register is set to DEVICE_ADDRESS
	reg_write_to_tsv(RFEND_CFG1, 0b00101110,UHFTSV);      //RFEND_CFG1: 0x2E       go to TX after a good packet, RX timeout disabled.
	//reg_write_to_tsv(0x29, 0b00111110,UHFTSV);			//RFEND_CFG1: 0x3E       go to RX after a good packet
	reg_write_to_tsv(RFEND_CFG0, 0b00110000,UHFTSV);      //RFEND_CFG0: 0x30       go to RX after transmitting a packet
	//reg_write_to_tsv(0x2A, 0b00100000,UHFTSV);			//RFEND_CFG0: 0x20       go to TX after transmitting a packet
	
	//set power level
	reg_write_to_tsv(PA_CFG2, 0b01111111,UHFTSV);			//PA_CFG2: 0x7F          set POWER_RAMP to 64 (output power to 14.5dBm, equation 21)
	
	//frequency offset setting
	reg_write2F_to_tsv(FREQOFF1, 0,UHFTSV);				//FREQOFF1: 0x00         set frequency offset to 0
	reg_write2F_to_tsv(FREQOFF0, 0,UHFTSV);				//FREQOFF0: 0x00
	
	//Frequency setting
	reg_write2F_to_tsv(FREQ2, 0x6C,UHFTSV);				//FREQ2: 0x6C            set frequency to 434MHz (sets Vco, see equation from FREQ2 section of user guide)
	reg_write2F_to_tsv(FREQ1, 0x80,UHFTSV);				//FREQ1: 0x80
	reg_write2F_to_tsv(FREQ0, 0x00,UHFTSV);				//FREQ0: 0x00
	return;
}

//This is a duplicate of upper setting function. Need to modified to VHF latter.
void reg_settings_VHF(void)
{
	//high performance settings
	reg_write2F_to_tsv(FS_DIG1, 0x00,VHFTSV);				//FS_DIG1: 0x00         Frequency Synthesizer Digital Reg. 1
	reg_write2F_to_tsv(FS_DIG0, 0x5F,VHFTSV);				//FS_DIG0: 0x5F         Frequency Synthesizer Digital Reg. 0
	reg_write2F_to_tsv(FS_CAL1, 0x40,VHFTSV);				//FS_CAL1: 0x40         Frequency Synthesizer Calibration Reg. 1
	reg_write2F_to_tsv(FS_CAL0, 0x0E,VHFTSV);				//FS_CAL0: 0x0E         Frequency Synthesizer Calibration Reg. 0
	reg_write2F_to_tsv(FS_DIVTWO, 0x03,VHFTSV);			//FS_DIVTWO: 0x03       Frequency Synthesizer Divide by 2
	reg_write2F_to_tsv(FS_DSM0, 0x33,VHFTSV);				//FS_DSM0: 0x33         FS Digital Synthesizer Module Configuration Reg. 0
	reg_write2F_to_tsv(FS_DVC0, 0x17,VHFTSV);				//FS_DVCO: 0x17         Frequency Synthesizer Divider Chain Configuration ..
	reg_write2F_to_tsv(FS_PFD, 0x50,VHFTSV);				//FS_PFD: 0x50          Frequency Synthesizer Phase Frequency Detector Con..
	reg_write2F_to_tsv(FS_PRE, 0x6E,VHFTSV);				//FS_PRE: 0x6E          Frequency Synthesizer Prescaler Configuration
	reg_write2F_to_tsv(FS_REG_DIV_CML, 0x14,VHFTSV);      //FS_REG_DIV_CML: 0x14  Frequency Synthesizer Divider Regulator Configurat..
	reg_write2F_to_tsv(FS_SPARE, 0xAC,VHFTSV);			//FS_SPARE: 0xAC        Set up Frequency Synthesizer Spare
	reg_write2F_to_tsv(FS_VCO0, 0xB4,VHFTSV);				//FS_VCO0: 0xB4         FS Voltage Controlled Oscillator Configuration Reg..
	reg_write2F_to_tsv(XOSC5, 0x0E,VHFTSV);				//XOSC5: 0x0E           Crystal Oscillator Configuration Reg. 5
	reg_write2F_to_tsv(XOSC1, 0x03,VHFTSV);				//XOSC1: 0x03           Crystal Oscillator Configuration Reg. 0
	
	/************************************/
	//For test purposes only, (2nd block, deleted first one) use values from SmartRF for some bits
	//High performance RX
	reg_write_to_tsv(SYNC_CFG1, 0x0B,VHFTSV);				//
	reg_write_to_tsv(DCFILT_CFG, 0x1C,VHFTSV);            //
	reg_write_to_tsv(IQIC, 0x00,VHFTSV);					//
	reg_write_to_tsv(CHAN_BW, 0x04,VHFTSV);				//
	reg_write_to_tsv(MDMCFG0, 0x05,VHFTSV);				//
	reg_write_to_tsv(AGC_CFG1, 0xA9,VHFTSV);				//
	reg_write_to_tsv(AGC_CFG0, 0xCF,VHFTSV);				//
	reg_write_to_tsv(FIFO_CFG, 0x00,VHFTSV);				//
	reg_write_to_tsv(SETTLING_CFG, 0x03,VHFTSV);          //
	reg_write2F_to_tsv(IF_MIX_CFG, 0x00,VHFTSV);          //
	/**************************************/
	
	//modulation and freq deviation settings
	reg_write_to_tsv(DEVIATION_M, 0b01001000,VHFTSV);     //DEVIATION_M: 0x48      set DEV_M to 72 which sets freq deviation to 20.019531kHz (with DEV_M=5)
	reg_write_to_tsv(MODCFG_DEV_E, 0b00000101,VHFTSV);    //MODCFG_DEV_E: 0x05     set up modulation mode and DEV_E to 5 (see DEV_M register)
	reg_write_to_tsv(FS_CFG, 0b00000100,VHFTSV);			//FS_CFG: B00010100      set up LO divider to 8 (410.0 - 480.0 MHz band), out of lock detector disabled
	
	//set preamble
	reg_write_to_tsv(PREAMBLE_CFG1, 0b00001101,VHFTSV);         //PREAMBLE_CFG1: 0x00    No preamble
	reg_write_bit_to_tsv(PREAMBLE_CFG0, 5, 1,VHFTSV);     //PQT_EN: 0x00           Preamble detection disabled
	
	//TOC_LIMIT
	reg_write_bit2F_to_tsv(TOC_CFG, 7, 0,VHFTSV);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	reg_write_bit2F_to_tsv(TOC_CFG, 6, 0,VHFTSV);			//TOC_LIMIT: 0x00      Using the low tolerance setting (TOC_LIMIT = 0) greatly reduces system settling times and system power consumption as no preamble bits are needed for bit synchronization or frequency offset compensation (4 bits preamble needed for AGC settling).
	
	//set SYNC word
	reg_write_bit_to_tsv(SYNC_CFG1, 6, 0,VHFTSV);			//PQT_GATING_EN: 0       PQT gating disabled (preamble not required)
	reg_write_to_tsv(SYNC_CFG0, 0x17,VHFTSV);				//SYNC_CFG0: B00010111   32 bit SYNC word. Bit error qualifier disabled. No check on bit errors
	reg_write_to_tsv(SYNC3, 0x93,VHFTSV);					//SYNC3: 0x93            Set SYNC word bits 31:24
	reg_write_to_tsv(SYNC2, 0x0B,VHFTSV);					//SYNC2: 0x0B            Set SYNC word bits 23:16
	reg_write_to_tsv(SYNC1, 0x51,VHFTSV);					//SYNC1: 0x51            Set SYNC word bits 15:8
	reg_write_to_tsv(SYNC0, 0xDE,VHFTSV);					//SYNC0: 0xDE            Set SYNC word bits 7:0
	
	cmd_str_to_transceiver(SNOP,VHFTSV);
	//set packets
	reg_write_bit_to_tsv(MDMCFG1, 6, 1,VHFTSV);			//FIFO_EN: 0             FIFO enable set to true
	reg_write_bit_to_tsv(MDMCFG0, 6, 0,VHFTSV);			//TRANSPARENT_MODE_EN: 0 Disable transparent mode
	reg_write_to_tsv(PKT_CFG2, 0b00000000,VHFTSV);		//PKT_CFG2: 0x00         set FIFO mode
	reg_write_to_tsv(PKT_CFG1, 0b00110000,VHFTSV);		//PKT_CFG1: 0x30         set address check and 0xFF broadcast
	reg_write_to_tsv(PKT_CFG0, 0b00100000,VHFTSV);		//PKT_CFG0: 0x30         set variable packet length
	reg_write_to_tsv(PKT_LEN, 0x7F,VHFTSV);				//PKT_LEN: 0xFF          set packet max packet length to 0x7F
	reg_write_to_tsv(DEV_ADDR, DEVICE_ADDRESS,VHFTSV);	//DEV_ADDR register is set to DEVICE_ADDRESS
	reg_write_to_tsv(RFEND_CFG1, 0b00101110,VHFTSV);      //RFEND_CFG1: 0x2E       go to TX after a good packet, RX timeout disabled.
	//reg_write_to_tsv(0x29, 0b00111110,VHFTSV);			//RFEND_CFG1: 0x3E       go to RX after a good packet
	reg_write_to_tsv(RFEND_CFG0, 0b00110000,VHFTSV);      //RFEND_CFG0: 0x30       go to RX after transmitting a packet
	//reg_write_to_tsv(0x2A, 0b00100000,VHFTSV);			//RFEND_CFG0: 0x20       go to TX after transmitting a packet
	
	//set power level
	reg_write_to_tsv(PA_CFG2, 0b01111111,VHFTSV);			//PA_CFG2: 0x7F          set POWER_RAMP to 64 (output power to 14.5dBm, equation 21)
	
	//frequency offset setting
	reg_write2F_to_tsv(FREQOFF1, 0,VHFTSV);				//FREQOFF1: 0x00         set frequency offset to 0
	reg_write2F_to_tsv(FREQOFF0, 0,VHFTSV);				//FREQOFF0: 0x00
	
	//Frequency setting
	reg_write2F_to_tsv(FREQ2, 0x6C,VHFTSV);				//FREQ2: 0x6C            set frequency to 434MHz (sets Vco, see equation from FREQ2 section of user guide)
	reg_write2F_to_tsv(FREQ1, 0x80,VHFTSV);				//FREQ1: 0x80
	reg_write2F_to_tsv(FREQ0, 0x00,VHFTSV);				//FREQ0: 0x00
	return;
}

void reg_settings_UHF_Beacon(void)
{
	reg_settings_UHF();
	reg_write_to_tsv(MODCFG_DEV_E, 0b00011100,UHFTSV); //Put the UHF transceiver to OOK
}

//set certain ss pin to low

void set_SS_low(uint8_t PIN)
{
	PIN_clr(PIN);
	delay_us(1);
}

//set certain ss pin to high
void set_SS_high(uint8_t PIN)
{
	PIN_set(PIN);
	delay_us(1);
}

uint8_t cmd_str_to_transceiver(uint8_t addr, uint8_t tsvNumber)
{
	uint8_t msg;
	set_SS_low(tsvNumber);
	msg = spi_transfer(addr);
	
	//delay_us(1);
	set_SS_high(tsvNumber);
	return msg;
}

uint8_t spi_transfer_to_device(uint8_t message, uint8_t deviceNumber)
{
	uint8_t* reg_ptr;
	uint8_t timeout = SPI_TIMEOUT;
	uint8_t receive_char;
	uint8_t i, temp, temp2;
	
	reg_ptr = SPDR_BASE;
	
	// Commence the SPI message.
	set_SS_low(deviceNumber);
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

void reg_write2F_to_tsv(uint8_t addr, uint8_t data, uint8_t tsvNumber)	
{
	cmd_str_to_transceiver(SNOP,tsvNumber);
	uint8_t msg, i, check;
	msg = 0b00101111;
	
	set_SS_low(tsvNumber);
	spi_transfer_to_device(msg,tsvNumber);
	//delay_us(1);
	msg = spi_transfer_to_device(addr,tsvNumber);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer_to_device(data,tsvNumber);		// Send the desired data
	set_SS_high(tsvNumber);
	//delay_ms(3);

	return;
}

void reg_write_to_tsv(uint8_t addr, uint8_t data, uint8_t tsvNumver)	
{
	uint8_t msg, check, i;
	
	set_SS_low(tsvNumver);
	msg = spi_transfer_to_device(addr,tsvNumver);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer_to_device(data,tsvNumver);		// Send the desired data
	set_SS_high(tsvNumver);
	//delay_ms(1);

	return;
}

uint8_t reg_read_to_tsv(uint8_t addr, uint8_t tsvNumber)
{
	uint8_t addr_new, msg;
	addr_new = addr + 0b10000000;

	set_SS_low(tsvNumber);
	msg = spi_transfer_to_device(addr_new,tsvNumber);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer_to_device(0x00,tsvNumber);
	set_SS_high(tsvNumber);
	//delay_ms(1);
	
	return msg;
}

uint8_t reg_read2F_to_device(uint8_t addr,uint8_t tsvNumber)
{
	uint8_t msg;
	msg = 0b10101111;
	
	set_SS_low(tsvNumber);
	msg = spi_transfer_to_device(msg,tsvNumber);
	//delay_us(1);
	msg = spi_transfer_to_device(addr,tsvNumber);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer_to_device(0x00,tsvNumber);
	set_SS_high(tsvNumber);
	//delay_ms(1);
	return msg;
}

void get_status_of_tsv(uint8_t *CHIP_RDYn, uint8_t *state, uint8_t tsvNumber)
{
	uint8_t msg;
	msg = cmd_str_to_transceiver(SNOP,tsvNumber);
	*CHIP_RDYn = (msg>>7)&1;
	*state = (msg>>4)&7;
	return;
}

uint8_t dir_FIFO_read_to_tsv(uint8_t addr, uint8_t tsvNumber)
{
	cmd_str_to_transceiver(SNOP,tsvNumber);
	uint8_t msg;
	msg = 0b10111110;
	
	set_SS_low(tsvNumber);
	msg = spi_transfer_to_device(msg,tsvNumber);
	//delay_us(1);
	msg = spi_transfer_to_device(addr,tsvNumber);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer_to_device(0x00,tsvNumber);
	set_SS_high(tsvNumber);
	//delay_ms(1);
	return msg;
}

void dir_FIFO_write_to_tsv(uint8_t addr, uint8_t data, uint8_t tsvNumber)
{
	cmd_str_to_transceiver(SNOP,tsvNumber);
	uint8_t msg;
	msg = 0b00111110;
	
	set_SS_low(tsvNumber);
	spi_transfer_to_device(msg,tsvNumber);
	//delay_us(1);
	msg = spi_transfer_to_device(addr,tsvNumber);		// Send the desired address
	//delay_us(1);
	msg = spi_transfer_to_device(data,tsvNumber);		// Send the desired data
	set_SS_high(tsvNumber);
	//delay_ms(3);
	
	return;
}

void reg_write_bit_to_tsv(uint8_t reg, uint8_t n, uint8_t data, uint8_t tsvNumber)
{
	uint8_t msg, temp;
	msg = reg_read_to_tsv(reg,tsvNumber);
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
	reg_write_to_tsv(reg, msg,tsvNumber);
	return;
}

void reg_write_bit2F_to_tsv(uint8_t reg, uint8_t n, uint8_t data,uint8_t tsvNumber)
{
	uint8_t msg, temp;
	msg = reg_read2F_to_device(reg,tsvNumber);
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
	reg_write2F_to_tsv(reg, msg,tsvNumber);
	return;
}

//Switch for VHF. 0 for receive. 1 for transmit
//Assume both switches turn on/off at the same time
void switchVHFset(bool mode)
{
	if(mode)
	{
		PIN_set(SW1_PIN);
		PIN_set(SW2_PIN);
	}
	else
	{
		PIN_clr(SW1_PIN);
		PIN_clr(SW2_PIN);
	}
}