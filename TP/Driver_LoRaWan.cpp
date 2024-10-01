#include "Driver_LoRaWan.hpp"

String appEui = SECRET_APP_EUI;   
String appKey = SECRET_APP_KEY;
String devEui = SECRET_DEV_EUI;

LoRaModem modem;
bool connected = false;
int err_count = 0;

void init_LoRaWan()
{
  if (!modem.begin(EU868)) 
  {
    Serial.println("Failed to start module");
    while (1) 
    {
      delay(1000);
    }
  }
}

void connect()
{
  Serial.println("trying to connect");
  
  int ret = modem.joinOTAA(appEui, appKey, devEui);
  
  if (ret)
  {
    connected = true;
    modem.minPollInterval(60);
    modem.dataRate(5);
    delay(100);
    err_count = 0;
  }
}

void send(char msg[], int size)
{
  int err = 0;
  modem.beginPacket();
  modem.write(msg, size);
  err = modem.endPacket(true);

  if (err <= 0)
  {
    Serial.println("erreur de transmission");
    err_count ++;
    if(err_count>50)
    {
      connected = false;
    }
      delay(1000);
  }
  else
  {
    Serial.println("transmission OK");
    err_count = 0;
  }
}
