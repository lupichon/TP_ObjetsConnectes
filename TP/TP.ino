
#include <Arduino.h> 
#include "Driver_SHT31.hpp"
#include "Driver_LoRaWan.hpp"
#include "Driver_Credentials.hpp"

void setup() 
{
  // Initialize serial communication
  Serial.begin(115200);
  SerialLoRa.begin(19200);
  while(!Serial && !SerialLoRa);
  
  //Initialize the LoRaWAN modem
  init_LoRaWan();

  //Initialize the SHT31 sensor
  init_SHT31();

  // Check if the credentials are already initialized
  if(!credentialsAlreadyInit())
  {
    // Initialize the NVM to the initial state
    writeNVM(0,MAGICNUMBER);
    writeNVM(1,0);
    writeNVM(2,MAGICNUMBER);

    // Initialize credentials with AT commands
    init_Credentials();
  }
}

void loop() 
{
  // Read the temperature and humidity from the SHT31 sensor
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  
  // Convert the floats into byte arrays
  uint8_t *bytePointerT = (uint8_t*)&t; 
  uint8_t *bytePointerH = (uint8_t*)&h;

  // Create a message array to send
  char msg[8] = {bytePointerT[0],bytePointerT[1],bytePointerT[2],bytePointerT[3],bytePointerH[0],bytePointerH[1],bytePointerH[2],bytePointerH[3]};

  // Check if the device is connected
  if(!connected)
  {
    connect();  // If not connected, try to connect
  }
  else
  {
    send(msg, sizeof(msg)); // Else, send the 8-byte message
  }

  delay(10000); // Wait 10 seconds before sending a new message for the duty cycle.
}

