//An arduino library by AISTLAB
//winxos 2016-05-11

#include <Arduino.h>
#include <AISTLAB.h>

//I2C address scan, using Serial print results.
void AISTLAB::I2C_address_scan()
{
  Wire.begin();
  Serial.begin(9600);
  int error, num = 0;
  for (int address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) //success
    {
      Serial.print("Found I2C at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
      num++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (num == 0)
    Serial.println("No I2C devices found.");
  else
  {
    Serial.print("Total found ");
    Serial.print(num);
    Serial.println(" I2C devices.");
  }
}
