#include "port_expander.h"

/************************************************************************/ 
/* Writes data to the device register with address register_address     */
/* TODO:                                                                */
/*   - Does the device indicate successful transmission anyway?         */
/*   - This code certainly does not work because I don't know the       */
/*     how spi_lib works (e.g. slave select, need to re-initialize      */
/*	   between transfers?) but does layout the methodology for          */
/*	   addressing the device                                            */
/************************************************************************/
void port_expander_write(uint8_t register_address, uint8_t data)
{
	spi_initialize_master();
	PIN_clr(SS_PIN);
	uint8_t write_control_byte = 0b01000000;
	if (A2 == 1) write_control_byte = write_control_byte | (0b00001000); 
	if (A1 == 1) write_control_byte = write_control_byte | (0b00000100); 
	if (A0 == 1) write_control_byte = write_control_byte | (0b00000010);
	spi_transfer(write_control_byte);
	spi_transfer(register_address);
	spi_transfer(data);
	PIN_set(SS_PIN); // close?
	return;
}

/************************************************************************/
/* Reads the data from the device register with address                 */     
/* register_address.                                                    */
/* TODO:                                                                */
/*   - Same as above                                                    */
/************************************************************************/
void port_expander_read(uint8_t register_address, uint8_t* data)
{
	spi_initialize_master();
	PIN_clr(SS_PIN);
	uint8_t write_control_byte = 0b01000001;
	if (A2 == 1) write_control_byte = write_control_byte | (0b00001000);
	if (A1 == 1) write_control_byte = write_control_byte | (0b00000100);
	if (A0 == 1) write_control_byte = write_control_byte | (0b00000010);
	spi_transfer(write_control_byte);
	*data = spi_transfer(register_address);
	PIN_set(SS_PIN); // close?
	return;
}