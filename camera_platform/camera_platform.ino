//frame work for remote control platform
//frame format: c len data checksum
//winxos 2016-08-17
#include <Servo.h>
Servo servo1;  // create servo object to control a servo

void setup() {
  servo1.attach(9);  // attaches the servo on pin 9 to
  Serial.begin(115200);
}
#define MAX_BUF 30
int buf[MAX_BUF]; //command frame buffer
int data_pt = 0; //buffer pointer
int state = 0; //reciever state
int frame_len = 0; //frame length
bool need_checksum = false; //whether need check sum.
byte check_sum(int *data, int len) //xor frame check sum
{
  int i, ans = 0;
  for (i = 0; i < len; i++)
  {
    ans ^= *(data + i);
  }
  return ans;
}
void run_cmd(int *data) //run after frame arrived
{
  switch (data[0])
  {
    case 'a': //server angle set: 'a n'
      servo1.write(data[1]);
      break;
  }
}
void get_cmd(int data)
{
  //Serial.print(state);
  switch (state)
  {
    case 0: // the first byte 'c' is the frame begin.
      if (data == 'c')
        state = 1;
      break;
    case 1: //the second byte is length, only data length not include checksum.
      frame_len = data;
      state = 2;
      data_pt = 0;
      break;
    case 2: //frame date
      buf[data_pt++] = data;
      if (data_pt >= frame_len)
        state = 3;
      break;
    case 3: //check sum
      if (!need_checksum)
      {
        run_cmd(buf);
      }
      else
      {
        if (check_sum(buf, frame_len) == data)
          run_cmd(buf);
      }
      state = 0;
      break;
  }
}
void loop() {
  if (Serial.available() > 0)
  {
    get_cmd(Serial.read());
  }
}
