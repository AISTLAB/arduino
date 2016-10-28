#include "public.h"
#include <EEPROM.h>

#define FILE_START 900
#define FILE_INFO_ADDR 924
#define FILE_INFO_LEN 10
#define MAX_FILE_NAME 8
#define MAX_FILE_NUM 10
#define MAX_FILE_LEN 256

void init_files()
{
  for (u16 i = FILE_INFO_ADDR; i < 1024; i++)
  {
    EEPROM.write(i, 0);
  }
}
void show_file_info()
{
  for (u16 i = FILE_INFO_ADDR; i < FILE_INFO_ADDR + 30; i++)
  {
    logs.print(EEPROM.read(i), HEX);
    logs.print(",");
  }
  logs.println();
  for (u16 i = FILE_START; i > FILE_START - 30; i--)
  {
    logs.print(EEPROM.read(i), HEX);
    logs.print(",");
  }
}
u8 get_last_file_index()
{
  u8 i;
  for (i = 0; i < MAX_FILE_NUM; i ++)
  {
    if (EEPROM.read(FILE_INFO_ADDR + i * FILE_INFO_LEN) == 0)
    {
      break;
    }
  }
  return i;
}
u16 get_size_info(u8 i)
{
  u16 addr = 0;
  addr = EEPROM.read(FILE_INFO_ADDR + i * FILE_INFO_LEN + 8);
  addr = addr << 8;
  addr += EEPROM.read(FILE_INFO_ADDR + i * FILE_INFO_LEN + 9);
  return addr;
}
u16 get_next_blank(u16 lastinfo)
{
  return ((lastinfo & 0xff80) >> 7) + (lastinfo & 0x007f);
}
bool save_file(String n, u16 * data, u8 len)
{
  u8 index = get_last_file_index();
  u16 st = 0;
  if (index > 0)
  {
    st = get_next_blank(get_size_info(index - 1));
  }
#ifdef DEBUG
  logs.print(index);
  logs.print(",");
  logs.println(st);
#endif
  for (u16 i = 0; i < len; i++)
  {
    EEPROM.write(FILE_START - st - i * 2, data[i] & 0x00ff);
    EEPROM.write(FILE_START - st - i * 2 - 1, data[i] >> 8);
  }
  for (u8 i = 0; i < n.length(); i++)
  {
    EEPROM.write(FILE_INFO_ADDR + index * FILE_INFO_LEN + i, n.charAt(i));
  }
  EEPROM.write(FILE_INFO_ADDR + index * FILE_INFO_LEN + 8, (u8)(st >> 1));
  EEPROM.write(FILE_INFO_ADDR + index * FILE_INFO_LEN + 9, (u8)((st & 0x0001) << 7) + len);
}
bool delete_file(String n)
{
}
u8 get_size(String n)
{

}
bool load_file(String n, u16 *out)
{

}
void get_files_name(String *out)
{

}

