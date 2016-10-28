#pragma once
#include <arduino.h>
#define M_UDP 0
#define M_SERIAL 1
#define M_ALL 2
class wLog {
  public:
    wLog()
    {
      method = M_SERIAL;
    }
    void log_method(char n)
    {
      method = n;
    }
    void print(uint8_t a, int b = DEC)
    {
      if (vailable == false)return;
      Serial.print(a, b);
    }
    void print(char& a, int b = DEC)
    {
      if (vailable == false)return;
      Serial.print(a, b);
    }
    void print(uint16_t a, int b = DEC)
    {
      if (vailable == false)return;
      Serial.print(a, b);
    }
    void print(String s)
    {
      if (vailable == false)return;
      switch (method)
      {
        case M_UDP:
          break;
        case M_SERIAL:
          Serial.print(s);
          break;
        case M_ALL:
          break;
      }
    }
    void println(String s)
    {
      Serial.println(s);
    }
    void println(uint8_t a, uint8_t b = DEC)
    {
      Serial.println(a, b);
    }
    void println(int a)
    {
      Serial.println(a);
    }
    void println(uint16_t a)
    {
      Serial.println(a);
    }
    void println()
    {
      print("\n");
    }
  private:
    char method;
    bool vailable;
};


