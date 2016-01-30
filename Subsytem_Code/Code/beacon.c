/*
 * beacon.c
 *
 * Created: 2016/1/30 14:42:29
 *  Author: Chris Zhang
 */ 

#include "beacon.h"

//This function takes one text array and transmit the morse code once.
void beacon_transmit(char text[])
{
	bool morse_message[1024] = { 0 };
	message2morse(text, morse_message);
	
	uint8_t message_counter=0;
	
	for(;message_counter<8;message_counter++)
	{
		for(unsigned i=0x00; i<128; i++)
		{
			dir_FIFO_write(i, morse_message[i+message_counter*128]);
		}
		reg_write2F(0xD3, 0x00);
		reg_write2F(0xD5, 0x7F);
		cmd_str(STX);
		message_counter++;
		delay_ms(2780);
	}
}

//convert one character to morse 'code' in hex form. Contents in switch block can be obtained from MorseGenerator.cpp

void char2code(char data, uint8_t *Length, uint8_t *code1,uint8_t *code2,uint8_t *code3, uint8_t *code4, uint8_t *code5, uint8_t *code6, uint8_t *code7,
      uint8_t *code8, uint8_t *code9, uint8_t *code10, uint8_t *code11, uint8_t *code12, uint8_t *code13, uint8_t *code14, uint8_t *code15, uint8_t *code16,
	  uint8_t* position_counter ){

switch (data){
case ' ':
*code1 = 0;
*code2 = 0;
*code3 = 0;
*Length = 21;
break;

case 'A':
*code1 = 255;
*code2 = 113;
*Length = 15;
break;

case 'B':
*code1 = 199;
*code2 = 113;
*code3 = 252;
*code4 = 7;
*Length = 27;
break;

case 'C':
*code1 = 199;
*code2 = 127;
*code3 = 28;
*code4 = 255;
*code5 = 1;
*Length = 33;
break;

case 'D':
*code1 = 199;
*code2 = 241;
*code3 = 31;
*Length = 21;
break;

case 'E':
*code1 = 7;
*Length = 3;
break;

case 'F':
*code1 = 199;
*code2 = 127;
*code3 = 28;
*code4 = 7;
*Length = 27;
break;

case 'G':
*code1 = 199;
*code2 = 127;
*code3 = 252;
*code4 = 7;
*Length = 27;
break;

case 'H':
*code1 = 199;
*code2 = 113;
*code3 = 28;
*Length = 21;
break;

case 'I':
*code1 = 199;
*code2 = 1;
*Length = 9;
break;

case 'J':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 113;
*Length = 39;
break;

case 'K':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*Length = 21;
break;

case 'L':
*code1 = 199;
*code2 = 241;
*code3 = 31;
*code4 = 7;
*Length = 27;
break;

case 'M':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*Length = 21;
break;

case 'N':
*code1 = 199;
*code2 = 127;
*Length = 15;
break;

case 'O':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 1;
*Length = 33;
break;

case 'P':
*code1 = 199;
*code2 = 127;
*code3 = 252;
*code4 = 199;
*code5 = 1;
*Length = 33;
break;

case 'Q':
*code1 = 255;
*code2 = 113;
*code3 = 252;
*code4 = 199;
*code5 = 127;
*Length = 39;
break;

case 'R':
*code1 = 199;
*code2 = 127;
*code3 = 28;
*Length = 21;
break;

case 'S':
*code1 = 199;
*code2 = 113;
*Length = 15;
break;

case 'T':
*code1 = 255;
*code2 = 1;
*Length = 9;
break;

case 'U':
*code1 = 255;
*code2 = 113;
*code3 = 28;
*Length = 21;
break;

case 'V':
*code1 = 255;
*code2 = 113;
*code3 = 28;
*code4 = 7;
*Length = 27;
break;

case 'W':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 7;
*Length = 27;
break;

case 'X':
*code1 = 255;
*code2 = 113;
*code3 = 28;
*code4 = 255;
*code5 = 1;
*Length = 33;
break;

case 'Y':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 199;
*code5 = 127;
*Length = 39;
break;

case 'Z':
*code1 = 199;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 1;
*Length = 33;
break;

case '0':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 241;
*code6 = 31;
*code7 = 255;
*code8 = 1;
*Length = 57;
break;

case '1':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 241;
*code6 = 31;
*code7 = 7;
*Length = 51;
break;

case '2':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 113;
*code6 = 28;
*Length = 45;
break;

case '3':
*code1 = 255;
*code2 = 241;
*code3 = 31;
*code4 = 199;
*code5 = 113;
*Length = 39;
break;

case '4':
*code1 = 255;
*code2 = 113;
*code3 = 28;
*code4 = 199;
*code5 = 1;
*Length = 33;
break;

case '5':
*code1 = 199;
*code2 = 113;
*code3 = 28;
*code4 = 7;
*Length = 27;
break;

case '6':
*code1 = 199;
*code2 = 113;
*code3 = 28;
*code4 = 255;
*code5 = 1;
*Length = 33;
break;

case '7':
*code1 = 199;
*code2 = 113;
*code3 = 252;
*code4 = 199;
*code5 = 127;
*Length = 39;
break;

case '8':
*code1 = 199;
*code2 = 241;
*code3 = 31;
*code4 = 255;
*code5 = 241;
*code6 = 31;
*Length = 45;
break;

case '9':
*code1 = 199;
*code2 = 127;
*code3 = 252;
*code4 = 199;
*code5 = 127;
*code6 = 252;
*code7 = 7;
*Length = 51;
break;



  default:
    break;
  }
}
//convert message to morse code
void message2morse(char message[], bool* morse)
{
  uint8_t current_position = 10;
  uint8_t length = 0;
  uint8_t code = 0;
  uint8_t code2 = 0;
  uint8_t code3 = 0;
  uint8_t code4 = 0;
  uint8_t code5 = 0;
  uint8_t code6 = 0;
  uint8_t code7 = 0;
  uint8_t code8 = 0;
  uint8_t code9 = 0;
  uint8_t code10 = 0;
  uint8_t code11 = 0;
  uint8_t code12 = 0;
  uint8_t code13 = 0;
  uint8_t code14 = 0;
  uint8_t code15 = 0;
  uint8_t code16 = 0;

	unsigned position_counter=0;
	
  for (int i = 0; message[i] != 0x00; i++)
  {
    char2code(message[i], &length, &code,&code2,&code3,&code4,&code5,&code6,&code7,&code8,&code9,&code10,&code11,&code12,&code13,&code14,&code15,&code16,&position_counter);
    code2morse(&current_position, code,code2,code3,code4, code5,code6,code7,code8,code9, code10,code11,code12,code13,code14, code15,code16, morse, length,&position_counter);

    int space_between_letter=9;
    
    if(current_position>(127-space_between_letter)){
      position_counter=position_counter+1;
      current_position=current_position-(128-space_between_letter);
    }
    else{
      current_position += space_between_letter;
    }
  }

};

//This function merges the morse code for each character to one array, which is ready to be transmitted

void code2morse(uint8_t *current, uint8_t code, uint8_t code2, uint8_t code3, uint8_t code4, uint8_t code5, uint8_t code6,
				 uint8_t code7, uint8_t code8, uint8_t code9, uint8_t code10, uint8_t code11, 
              uint8_t code12, uint8_t code13, uint8_t code14, uint8_t code15, uint8_t code16, bool* morse, uint8_t length,
			  uint8_t* position_counter)
{
  uint8_t bits[128] = { 0 };
  
  uint8_t remain_bit = (length % 8) == 0 ? 8 : (length % 8);
if (length>120)
  {
	//The first for loop fills the last byte of the morse code
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code16 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+120] = thebit;
    }
	//The rest for loops fill the rest part of the morse code
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code15 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+112] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code14 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+104] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code13 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+96] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code12 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+88] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code11 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+80] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }if (length>112)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code15 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+112] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code14 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+104] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code13 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+96] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code12 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+88] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code11 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+80] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>104)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code14 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+104] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code13 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+96] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code12 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+88] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code11 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+80] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>96)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code13 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+96] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code12 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+88] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code11 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+80] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>88)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code12 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+88] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code11 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+80] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>80)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code11 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+80] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>72)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code10 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+72] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>64)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code9 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+64] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>56)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code8 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+56] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>48)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code7 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+48] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  
else if (length>40)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code6 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+40] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>32)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code5 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+32] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>24)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code4 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+24] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>16)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code3 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+16] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else if (length>8)
  {
    for (uint8_t k = remain_bit-1; k>=0 && k!=255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code2 & mask;
      uint8_t thebit = masked_n >> k;
      bits[k+8] = thebit;
    }
    for (uint8_t k = 7; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }
  else
  {
    for (uint8_t k = remain_bit - 1; k>=0 && k != 255; k--) {
      uint8_t mask = 1 << k;
      uint8_t masked_n = code & mask;
      uint8_t thebit = masked_n >> k;
      bits[k] = thebit;
    }
  }


  for (uint8_t i = 0; i<length; i++)
  {
    morse[*current + i+128*(*position_counter)] |= bits[length-1-i];
  }
  //*current += length;

  if(*current>(127-length)){
      *position_counter=*position_counter+1;
      *current=*current-(128-length);
    }
    else{
      *current += length;
    }

}