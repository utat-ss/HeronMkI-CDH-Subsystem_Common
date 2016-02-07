#include "port.h"

#define SS_PIN 0 /* Slave select pin */
#define A2 0 /* Logic state of the address pins on the port expander */
#define A1 0
#define A0 0
#define INPUT 1 /* Enumerations for setting pin mode */
#define OUTPUT 0

#define IODIR_BASE 0x00 /* Base addresses of useful registers */
#define GPIO_BASE  0x12
#define IOCON      0x0A // MCP23x17 Configuration Register

/************************************************************************/
/* Initializes the port expander. Must be called before use.            */
/************************************************************************/
void port_expander_init()

/************************************************************************/
/* Writes data to the device register with address register_address     */
/************************************************************************/
void port_expander_write(uint8_t register_address, uint8_t data);

/************************************************************************/
/* Reads the data from the device register with address                 */
/* register_address.                                                    */
/************************************************************************/
void port_expander_read(uint8_t register_address, uint8_t* data);

/************************************************************************/
/* Sets gpio A's pin to input or output (io should be 0 or 1)           */
/* Parameters:                                                          */
/*   id - number between 0 and 7                                        */
/*   io - INPUT or OUTPUT (1 or 0)                                      */
/************************************************************************/
void gpioa_pin_mode(uint8_t id, uint8_t io);

/************************************************************************/
/* Sets gpio B's pin to input or output (io should be 0 or 1)           */
/************************************************************************/
void gpiob_pin_mode(uint8_t id, uint8_t io);

/************************************************************************/
/* Sets gpio A's (puts it to high)                                      */
/************************************************************************/
void set_gpioa_pin(uint8_t id);

/************************************************************************/
/* Clears gpio A's (puts it to low)                                     */
/************************************************************************/
void clr_gpioa_pin(uint8_t id);

/************************************************************************/
/* Sets gpio B's (puts it to high)                                      */
/************************************************************************/
void set_gpiob_pin(uint8_t id);

/************************************************************************/
/* Clears gpio B's (puts it to low)                                     */
/************************************************************************/
void clr_gpiob_pin(uint8_t id);

/************************************************************************/
/* Reads state of gpio A pin specified by id (0 or 1)                   */
/************************************************************************/
uint8_t read_gpioa_pin(uint8_t id);

/************************************************************************/
/* Reads state of gpio B pin specified by id (0 or 1)                   */
/************************************************************************/
uint8_t read_gpiob_pin(uint8_t id);