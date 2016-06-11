/*
 * beacon.h
 *
 * Created: 2016/1/30 14:42:40
 *  Author: Chris Zhang, Bill
 */ 
#include <stdint.h>
#include <stdbool.h>

#define MAX_CHAR_LEN 100	//can easily hold any single character
#define DOT_LEN 3
#define DASH_LEN 9
#define SPACE_LEN 3			//length of a space (9+3+9 so for each space there will be 21 zeroes)

#define BREAK_LEN 3 		//length between each dot/dash
#define CHAR_BREAK_LEN 6 	//length between each character (6+3 so between each character will be 9 zeroes)

#define DOT '.'
#define DASH '-'
#define SPACE ' '

//This function takes one text array and transmit the morse code once
//Can accept uppercase and lowercase letters, alphanumerics, and spaces
void beacon_transmit(char text[]);

//send the given array to the transceiver
void send_array(bool array[], uint8_t len);
//converts the character to morse code
void get_morse(char c, bool morse_out[], uint8_t* out_len);
//converts the given character to a string of dots and dashes
void get_dots_and_dashes_from_char(char in, char* out, uint8_t* out_length);
//converts the string of dots and dashes to an array of booleans
void get_bools_from_dots_and_dashes(char* in, uint8_t in_len, bool* out, uint8_t* out_len);