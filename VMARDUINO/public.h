#pragma once

typedef unsigned char u8;
typedef unsigned int u16;

#define SET_IO_OUTPUT 0x10
#define SET_IO_INPUT 0x11
#define SET_DELAY_GAP 0x12
#define READ_IO_DIGITAL 0x20
#define READ_IO_ANALOG 0x21
#define WRITE_IO_DIGITAL_HIGH 0x22
#define WRITE_IO_DIGITAL_LOW 0x23
#define WRITE_IO_ANALOG 0x24
#define LOAD_CONST 0x29
#define LOAD_MEM 0x2A
#define SAVE_TO_MEM 0x2B
#define LOAD_EEPROM 0x2C
#define SAVE_TO_EEPROM 0x2D
#define ADD 0x30
#define SUB 0x31
#define MUL 0x32
#define DIV 0x33
#define MOD 0x34
#define JMP 0x40
#define JMPN 0x41
#define JMPZ 0x42
#define DELAY 0x4A
#define HALT 0x4B
#define SERIAL_PRINT 0x50
#define UDP_PRINT 0x51
#define SER 0x60


void get_data();
bool receive_data(u8 data);
u8 stepSML();

