#include "port_expander.h"

void port_expander_write(uint8_t register_address, uint8_t data)
{
	PIN_clr(SS_PIN);
	uint8_t write_control_byte = 0b01000000;
	if (A2 == 1) write_control_byte = write_control_byte | (0b00001000); 
	if (A1 == 1) write_control_byte = write_control_byte | (0b00000100); 
	if (A0 == 1) write_control_byte = write_control_byte | (0b00000010);
	spi_transfer(write_control_byte);
	spi_transfer(register_address);
	spi_transfer(data);
	PIN_set(SS_PIN);
	return;
}

void port_expander_read(uint8_t register_address, uint8_t* data)
{
	PIN_clr(SS_PIN);
	uint8_t write_control_byte = 0b01000001;
	if (A2 == 1) write_control_byte = write_control_byte | (0b00001000);
	if (A1 == 1) write_control_byte = write_control_byte | (0b00000100);
	if (A0 == 1) write_control_byte = write_control_byte | (0b00000010);
	spi_transfer(write_control_byte);
	*data = spi_transfer(register_address);
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
	if ( (state & (1 << id) ) == 1 )
	{
		return 1;	
	}
	else
	{
		
		return 0;
	}
}

void read_gpiob_pin(uint8_t id, uint8_t* data)
{
	uint8_t state;
	port_expander_read(GPIO_BASE + 0x01, &state);
	if ( (state & (1 << id) ) == 1 )
	{
		return 1;
	}
	else
	{
		
		return 0;
	}
}