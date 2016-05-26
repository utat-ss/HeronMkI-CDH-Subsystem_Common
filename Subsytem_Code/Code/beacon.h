/*
 * beacon.h
 *
 * Created: 2016/1/30 14:42:40
 *  Author: Chris Zhang
 */ 
#include <stdint.h>
#include <stdbool.h>
#include "trans_lib.h"

#define BOOL_MSG_SIZE 1024

//This function takes one text array and transmit the morse code once.
void beacon_transmit(char text[]);

//convert one character to morse 'code' in hex form. Contents in switch block can be obtained from MorseGenerator.cpp
void char2code(char data, uint8_t *Length, uint8_t *code1,uint8_t *code2,uint8_t *code3, uint8_t *code4, uint8_t *code5, uint8_t *code6, uint8_t *code7,
uint8_t *code8, uint8_t *code9, uint8_t *code10, uint8_t *code11, uint8_t *code12, uint8_t *code13, uint8_t *code14, uint8_t *code15, uint8_t *code16,
uint8_t* position_counter );

//convert message to morse code
void message2morse(char message[], bool* morse);

//This function merges the morse code for each character to one array, which is ready to be transmitted
void code2morse(uint8_t *current, uint8_t code, uint8_t code2, uint8_t code3, uint8_t code4, uint8_t code5, uint8_t code6,
uint8_t code7, uint8_t code8, uint8_t code9, uint8_t code10, uint8_t code11,
uint8_t code12, uint8_t code13, uint8_t code14, uint8_t code15, uint8_t code16, bool* morse, uint8_t length,
uint8_t* position_counter);
