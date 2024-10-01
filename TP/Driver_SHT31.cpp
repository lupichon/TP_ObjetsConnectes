#include "Driver_SHT31.hpp"

Adafruit_SHT31 sht31 = Adafruit_SHT31();

void init_SHT31()
{
  if (! sht31.begin(0x44)) 
  {   
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}
