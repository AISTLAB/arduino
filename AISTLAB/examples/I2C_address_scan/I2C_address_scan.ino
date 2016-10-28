//I2C address scan example
//winxos 2016-05-11
#include <AISTLAB.h>
AISTLAB ww;
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  ww.I2C_address_scan();
  delay(1000);
}