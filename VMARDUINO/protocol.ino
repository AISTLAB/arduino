/*
PROTOCOL:
ASK:
MARK_START IP MARK_START LEN CMD [DATA_ADDR[,DATA_LEN] [DATA]] CRC MARK_END
REPLY:
MARK_START IP MARK_START LEN CMD [DATA_ADDR[,DATA_LEN] DATA] CRC MARK_END
CMD:
get/set eeprom(flash storage)
get/set ram(sensors and other)
get/set program(dynamic field)
WINXOS 2014-05-05
*/
#include "public.h"

#define MARK_START 0x68
#define MARK_END 0x16
#define BUF_MAX_LEN 0xA0

enum {NOT_READY, SUCCESS};
enum {  S_READY, S_IP, S_LEN, S_DATA, S_VERIFY, S_END};

struct FRAME {
  u8 ip;
  u8 len;
  u8 counter;
  u8 data[BUF_MAX_LEN];
} buf;
u8 receive_state = S_READY;
extern u8 state;
void action()
{
  Serial.println("VM loading...");
  initSML();
  loadCode((u16*)buf.data,buf.len/2);
  state=1;
}

//validate
bool verify(u8 n)
{
  u8 sum = 0;
  for (u8 i = 0; i < buf.len; i++)
  {
    sum += buf.data[i];
  }
  #ifdef DEBUG
  logs.print("Crc:");
  logs.println(sum,HEX);
  #endif
  return n == sum;
}
//error bytes recovery
u8 fail_state(u8 n)
{
  if (n == MARK_START)
  {
    return S_IP;
  }
  return S_READY;
}
//recieve the frame
bool receive_data(u8 data)
{
#ifdef DEBUG
  logs.print(data, DEC);
  logs.print(",");
  logs.println(receive_state);
#endif
  switch (receive_state)
  {
    case S_READY:
      if (data == MARK_START)
      {
        receive_state = S_IP;
      }
      break;
    case S_IP:
      buf.ip = data;
      receive_state = S_LEN;
      break;
    case S_LEN:
      buf.len = data;
      buf.counter = 0;
      receive_state = S_DATA;
      break;
    case S_DATA:
      buf.data[buf.counter++] = data;
      if (buf.counter == buf.len)
      {
        receive_state = S_VERIFY;
      }
      break;
    case S_VERIFY:
      if (verify(data) == true)
      {
        receive_state = S_END;
      }
      else
      {
        receive_state = fail_state(data);
      }
      break;
    case S_END:
      if (data == MARK_END)
      {
        receive_state = S_READY;
        action();
        return SUCCESS;
      }
      else
      {
        receive_state = fail_state(data);
      }
      break;
    default: break;
  }
  return NOT_READY;
}

