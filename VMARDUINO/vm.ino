#ifndef _ws_vm
#define _ws_vm
/*
SML(simple machine language) simulator Simpletron DEMO.
Language Format: xxbb
The Lower 2 digits bb is memory address.
The Higher 2 digits xx is the operater.
Operators: READ, WRITE, LOAD, STORE, ADD, SUB, MUL, DIV, MOD, JMP, JMPN, JMPE, HALT
winxos 2012-11-04

updater:
#define SET_IO_OUTPUT 0x10
#define SET_IO_INPUT 0x11
#define READ_IO_DIGITAL 0x20
#define READ_IO_ANALOG 0x21
#define WRITE_IO_DIGITAL_HIGH 0x22
#define WRITE_IO_DIGITAL_LOW 0x23
#define WRITE_IO_ANALOG 0x24
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
*/
#include "public.h"
#include <Servo.h>

Servo myservo[10];
#define MAX_MEM 100
u16 mem[MAX_MEM + 1] = {0}, ADDER = 0; //ONLY ONE REGISTER ADDER
u16 time_min = 1;
unsigned long time_mark = 0;
u8 time_state = 0;
u16 op, addr, *pt;
u8 state = 0;
void loadCode(u16 c[], byte len)
{
  memset(mem, 0, sizeof(u16) * (1 + MAX_MEM));
  memcpy(mem, c, len * sizeof(u16));
  for (int i = 0; i < len; i++)
  {
    Serial.println(mem[i], HEX);
  }
  mem[MAX_MEM] = HALT << 8;
}
void initSML()
{
  op = 0, addr = 0, pt = mem;
  for(int i=0;i<10;i++)
  {
    myservo[i].attach(i);
  }
}
u8 stepSML() //
{
  if (state == 0) return 0;
  op = (*pt) >> 8;	//xxbb
  addr = (*pt) & 0x00FF;
  pt++;
#ifdef DEBUG
  logs.print(op, HEX);
  logs.print(",");
  logs.print(addr, HEX);
  logs.print(",");
  logs.println(ADDER);
#endif
  switch (op)
  {
    case SET_IO_OUTPUT: pinMode(addr, OUTPUT); break;
    case SET_IO_INPUT: pinMode(addr, INPUT); break;
    case SET_DELAY_GAP: time_min = addr; break;
    case READ_IO_DIGITAL: ADDER = digitalRead(addr); break;
    case READ_IO_ANALOG: ADDER = analogRead(addr); break;
    case WRITE_IO_DIGITAL_HIGH: digitalWrite(addr, HIGH); break;
    case WRITE_IO_DIGITAL_LOW: digitalWrite(addr, LOW); break;
    case WRITE_IO_ANALOG: analogWrite(addr, ADDER); break;
    case LOAD_CONST: ADDER = addr; break;
    case LOAD_MEM: ADDER = mem[addr]; break;
    case SAVE_TO_MEM: mem[addr] = ADDER; break;
    case LOAD_EEPROM: break;
    case SAVE_TO_EEPROM: break;
    case ADD: ADDER += mem[addr]; break;
    case SUB: ADDER -= mem[addr]; break;
    case MUL: ADDER *= mem[addr]; break;
    case DIV: ADDER /= mem[addr]; break;
    case MOD: ADDER %= mem[addr]; break;
    case JMP: pt = mem + addr; break;
    case JMPN: ADDER < 0 ? pt = mem + addr : 0; break;
    case JMPZ: ADDER == 0 ? pt = mem + addr : 0; break;
    case DELAY:
      switch (time_state)
      {
        case 0:
          time_mark = millis();
          time_state = 1;
          pt--;
          break;
        case 1:
          if ((millis() - time_mark) > time_min * addr) //end delay
          {
            time_state = 0;
          }
          else //normal delaying
          {
            pt--;
          }
          break;
      }
      break;
    case HALT: state = 0; Serial.println("\nhalt"); return 0;
    case SERIAL_PRINT: break;
    case UDP_PRINT: break;
    case SER: myservo[addr].write(ADDER);Serial.println(ADDER); break;
    default: break;
  }
  return 1;
}
#endif

