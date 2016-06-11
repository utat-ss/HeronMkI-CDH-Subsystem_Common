/*
 * beacon.c
 *
 * Created: 2016/1/30 14:42:29
 *  Author: Chris Zhang, Bill
 */ 

#include "beacon.h"

//This function takes one text array and transmit the morse code once.
void beacon_transmit(char text[])
{
	uint8_t msg_counter=0;
	
	bool morse_char[MAX_CHAR_LEN] = { 0 }; //big enough for any one character
	uint8_t morse_len=0;
	
	bool morse_msg[128] = { 0 };
	
	int i=0;
	for(; text[i]!=0x00; i++)
	{
		get_morse(text[i], morse_char, &morse_len);
		
		//try to add the current message to the array
		int k=0;
		for (; k<morse_len; k++)
		{
			if (msg_counter >= 128)
			{
				//generate 128 bits of data at a time
				//send what we have, then restart the array
				send_array(morse_msg, msg_counter);
				msg_counter=0;
			}
			
			morse_msg[msg_counter] = morse_char[k];
			msg_counter++;
		}
	}
	
	//send the last bit
	if (msg_counter != 0)
		send_array(morse_msg, msg_counter);
	
}

void send_array(bool array[], uint8_t len)
{
	//(copied from Chris's code, I'm assuming it is correct)
	
	uint8_t j;
	for (j=0; j<len; j++)
	{
		dir_FIFO_write(j, array[j]);
	}
	reg_write2F(0xD3, 0x00);
	reg_write2F(0xD5, 0x7F);
	cmd_str(STX);
	delay_ms(2780);
	
}

void get_morse(char c, bool morse_out[], uint8_t* out_len)
{
	uint8_t length;
	char dots_and_dashes[5];
	
	get_dots_and_dashes_from_char(c, dots_and_dashes, &length);
	get_bools_from_dots_and_dashes(dots_and_dashes, length, morse_out, out_len); 	
	
	return;
}

void get_dots_and_dashes_from_char(char in, char* out, uint8_t* out_length)
{
switch (in){
case ' ':
*out_length=1;
out[0] = SPACE;
break;

case 'A':
case 'a':
*out_length=2;
out[0]=DOT;
out[1]=DASH;
break;

case 'B':
case 'b':
*out_length=4;
out[0]=DASH;
out[1]=DOT;
out[2]=DOT;
out[3]=DOT;
break;

case 'C':
case 'c':
*out_length=4;
out[0]=DASH;
out[1]=DOT;
out[2]=DASH;
out[3]=DOT;
break;

case 'D':
case 'd':
*out_length=3;
out[0]=DASH;
out[1]=DOT;
out[2]=DOT;
break;

case 'E':
case 'e':
*out_length=1;
out[0]=DOT;
break;

case 'F':
case 'f':
*out_length=4;
out[0]=DOT;
out[1]=DOT;
out[2]=DASH;
out[3]=DOT;
break;

case 'G':
case 'g':
*out_length=3;
out[0]=DASH;
out[1]=DASH;
out[2]=DOT;
break;

case 'H':
case 'h':
*out_length=4;
out[0]=DOT;
out[1]=DOT;
out[2]=DOT;
out[3]=DOT;
break;

case 'I':
case 'i':
*out_length=2;
out[0]=DOT;
out[1]=DOT;
break;

case 'J':
case 'j':
*out_length=4;
out[0]=DOT;
out[1]=DASH;
out[2]=DASH;
out[3]=DASH;
break;

case 'K':
case 'k':
*out_length=3;
out[0]=DASH;
out[1]=DOT;
out[2]=DASH;
break;

case 'L':
case 'l':
*out_length=4;
out[0]=DOT;
out[1]=DASH;
out[2]=DOT;
out[3]=DOT;
break;

case 'M':
case 'm':
*out_length=2;
out[0]=DASH;
out[1]=DASH;
break;

case 'N':
case 'n':
*out_length=2;
out[0]=DASH;
out[1]=DOT;
break;

case 'O':
case 'o':
*out_length=3;
out[0]=DASH;
out[1]=DASH;
out[2]=DASH;
break;

case 'P':
case 'p':
*out_length=4;
out[0]=DOT;
out[1]=DASH;
out[2]=DASH;
out[3]=DOT;
break;

case 'Q':
case 'q':
*out_length=4;
out[0]=DASH;
out[1]=DASH;
out[2]=DOT;
out[3]=DASH;
break;

case 'R':
case 'r':
*out_length=3;
out[0]=DOT;
out[1]=DASH;
out[2]=DOT;
break;

case 'S':
case 's':
*out_length=3;
out[0]=DOT;
out[1]=DOT;
out[2]=DOT;
break;

case 'T':
case 't':
*out_length=1;
out[0]=DASH;
break;

case 'U':
case 'u':
*out_length=3;
out[0]=DOT;
out[1]=DOT;
out[2]=DASH;
break;

case 'V':
case 'v':
*out_length=4;
out[0]=DOT;
out[1]=DOT;
out[2]=DOT;
out[3]=DASH;
break;

case 'W':
case 'w':
*out_length=3;
out[0]=DOT;
out[1]=DASH;
out[2]=DASH;
break;

case 'X':
case 'x':
*out_length=4;
out[0]=DASH;
out[1]=DOT;
out[2]=DOT;
out[3]=DASH;
break;

case 'Y':
case 'y':
*out_length=4;
out[0]=DASH;
out[1]=DOT;
out[2]=DASH;
out[3]=DASH;
break;

case 'Z':
case 'z':
*out_length=4;
out[0]=DASH;
out[1]=DASH;
out[2]=DOT;
out[3]=DOT;
break;

case '0':
*out_length=5;
out[0]=DASH;
out[1]=DASH;
out[2]=DASH;
out[3]=DASH;
out[4]=DASH;
break;

case '1':
*out_length=5;
out[0]=DOT;
out[1]=DASH;
out[2]=DASH;
out[3]=DASH;
out[4]=DASH;
break;

case '2':
*out_length=5;
out[0]=DOT;
out[1]=DOT;
out[2]=DASH;
out[3]=DASH;
out[4]=DASH;
break;

case '3':
*out_length=5;
out[0]=DOT;
out[1]=DOT;
out[2]=DOT;
out[3]=DASH;
out[4]=DASH;
break;

case '4':
*out_length=5;
out[0]=DOT;
out[1]=DOT;
out[2]=DOT;
out[3]=DOT;
out[4]=DASH;
break;

case '5':
*out_length=5;
out[0]=DOT;
out[1]=DOT;
out[2]=DOT;
out[3]=DOT;
out[4]=DOT;
break;

case '6':
*out_length=5;
out[0]=DASH;
out[1]=DOT;
out[2]=DOT;
out[3]=DOT;
out[4]=DOT;
break;

case '7':
*out_length=5;
out[0]=DASH;
out[1]=DASH;
out[2]=DOT;
out[3]=DOT;
out[4]=DOT;
break;

case '8':
*out_length=5;
out[0]=DASH;
out[1]=DASH;
out[2]=DASH;
out[3]=DOT;
out[4]=DOT;
break;

case '9':
*out_length=5;
out[0]=DASH;
out[1]=DASH;
out[2]=DASH;
out[3]=DASH;
out[4]=DOT;
break;



  default:
  *out_length = 0;
    break;
  }
}

void get_bools_from_dots_and_dashes(char* in, uint8_t in_len, bool* out, uint8_t* out_len)
{
	uint8_t in_counter=0, out_counter=0, i=0;
	for (; in_counter<in_len; in_counter++)
	{
		switch(in[in_counter])
		{
			case DOT:
				for (i=0; i<DOT_LEN; i++)
				{
					out[out_counter+i] = 1;
				}
				out_counter+=i;
				break;
				
			case DASH:
				for (i=0; i<DASH_LEN; i++)
				{
					out[out_counter+i] = 1;
				}
				out_counter+=i;
				break;
				
			case SPACE:
				for (i=0; i<SPACE_LEN; i++)
				{
					out[out_counter+i] = 0;
				}
				out_counter+=i;
				break;
				
			default:
				
				break;
		}
		
		//add the break between dots/dashes
		for (i=0; i<BREAK_LEN; i++)
		{
			out[out_counter+i] = 0;
		}
		out_counter+=i;
	}
	
	//add the break between characters
	for (i=0; i<CHAR_BREAK_LEN; i++)
	{
		out[out_counter+i] = 0;
	}
	out_counter+=i;
	
	*out_len = out_counter;
}

