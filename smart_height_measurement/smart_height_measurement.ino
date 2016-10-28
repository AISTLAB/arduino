/*
智能超声波身高测量仪
采用讯飞XFS5152进行语音播报
arduino 1.6.7
Sleep_n0m1库 从包管理器下载
encoding 完成utf8 -> unicode 编码转换
winxos 2016-05-07
*/
#include <LiquidCrystal_I2C.h>
#include <Sleep_n0m1.h>
#include "encoding.h"
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
Sleep sleep;
int base = 188, bias = 6;
void tts(uint8_t *str) {
  unsigned  char Frame_Info[50];
  unsigned  int  HZ_Length;
  unsigned  int i = 0;
  HZ_Length = strlen((const char *)str); 
  Frame_Info[0] = 0xFD ;             
  Frame_Info[1] = 0x00 ;             //构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 2;     //构造数据区长度的低字节
  Frame_Info[3] = 0x01;          //构造命令字：合成播放命令
  Frame_Info[4] = 0x03;       //文本编码格式：UNICODE
  memcpy(&Frame_Info[5], str, HZ_Length);
  Serial.write(Frame_Info, 5 + HZ_Length);
}
void ttsen(uint8_t *str) {
  unsigned  char Frame_Info[50]; //定义的文本长度
  unsigned  int  HZ_Length;
  unsigned  int i = 0;
  HZ_Length = strlen((const char *)str);          //需要发送文本的长度
  Frame_Info[0] = 0xFD ;             //构造帧头FD
  Frame_Info[1] = 0x00 ;             //构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 2;     //构造数据区长度的低字节
  Frame_Info[3] = 0x01;          //构造命令字：合成播放命令
  Frame_Info[4] = 0x01;       //文本编码格式：GBK
  memcpy(&Frame_Info[5], str, HZ_Length);
  Serial.write(Frame_Info, 5 + HZ_Length);
}
void ttsnum(int n) {
  char buf[10] = {"[h1]"};
  itoa(n, &buf[4], 10);
  ttsen((uint8_t*)buf);
}
void say(int n)
{
  tts((uint8_t *)utf82unicode("您的身高"));
  delay(2000);
  ttsnum(n);
  delay(1000);
  tts((uint8_t *)utf82unicode("厘米"));
  delay(500);
}
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("AISTLAB");
  tts((uint8_t *)utf82unicode("欢迎来到"));
  ttsen((uint8_t *)("AISTLAB"));
  delay(3000);
  pinMode(7, OUTPUT);
  Serial.begin(9600);
  int t = get_distance();
  if (t > 180) {
    base = t;
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("base height:");
    lcd.setCursor(0, 1);
    lcd.print(t);
    lcd.print("    ");
    lcd.setCursor(10, 1);
    lcd.print("cm   ");
    tts((uint8_t *)utf82unicode("安装高度"));
    delay(2000);
    ttsnum(base);
    delay(1000);
    tts((uint8_t *)utf82unicode("厘米，欢迎使用智能身高测量仪。"));
    delay(3000);
    sleep.pwrDownMode(); //set sleep mode
    sleep.sleepDelay(5000); //sleep for: sleepTime
    lcd.noBacklight();
  }
}

int get_distance() {
  digitalWrite(7, LOW);
  delayMicroseconds(2);
  digitalWrite(7, HIGH);
  delayMicroseconds(20);
  digitalWrite(7, LOW);
  float Fdistance = pulseIn(8, HIGH, 30000);
  Fdistance = Fdistance / 58; //cm
  return (int)(Fdistance + 0.5);
}
void show_msg(int t) {
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("distance:");
  lcd.setCursor(0, 1);
  lcd.print(t);
  lcd.print("    ");
  lcd.setCursor(10, 1);
  lcd.print("cm   ");
}
int n = 160;
void loop() {
  int t = get_distance();
  if (base - t + bias > 120) {
    show_msg(base - t + bias);
    say(base - t + bias);
  } else {
    lcd.noBacklight();
    sleep.pwrDownMode(); //set sleep mode
    sleep.sleepDelay(2000); //sleep for: sleepTime
  }
  delay(500);
}
