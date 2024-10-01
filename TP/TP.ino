
#include <Arduino.h> 
#include "Driver_SHT31.hpp"
#include "Driver_LoRaWan.hpp"

void setup() 
{
  
  Serial.begin(115200);
  while(!Serial);

  init_SHT31();
  init_LoRaWan();
}

void loop() 
{
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
 
  uint8_t *bytePointerT = (uint8_t*)&t; 
  uint8_t *bytePointerH = (uint8_t*)&h;
  char msg[8] = {bytePointerT[0],bytePointerT[1],bytePointerT[2],bytePointerT[3],bytePointerH[0],bytePointerH[1],bytePointerH[2],bytePointerH[3]};

  if(!connected)
  {
    connect();
  }

  if (connected)
  {
    send(msg, sizeof(msg));
  }
  delay(10000);
}
