//btn test
#include "btn.h"
void btn1_pressed()
{
  Serial.println("btn1 pressed.");
}
void btn2_up()
{
  Serial.println("btn2 up.");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  add_keydown_listener(3, &btn1_pressed);//binded to pin 3
  add_keyup_listener(4, &btn2_up);//binded to pin 4
  delay(1000);
}
void loop() {
  // put your main code here, to run repeatedly:
  btn_scan();
  delay(10);
}
