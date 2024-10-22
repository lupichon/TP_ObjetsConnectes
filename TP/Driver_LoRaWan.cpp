/*
 * File: Driver_LoRaWan.cpp
 * 
 * Description:
 * This source implements the driver for managing 
 * LoRaWAN communication using the MKR WAN 1310 board. It includes functions 
 * for initializing the modem, connecting to the LoRaWAN network via OTAA 
 * (Over The Air Activation), and sending data through the network.
 * 
 * Functions:
 * - init_LoRaWan: Initializes the LoRaWAN modem and starts communication 
 *   with the module. If initialization fails, the function enters an infinite loop.
 * 
 * - connect: Attempts to connect the device to the LoRaWAN network using the 
 *   provided AppEUI, AppKey, and DevEUI credentials. If the connection is 
 *   successful, it configures the modem for polling and data rate settings.
 * 
 * - send: Sends a packet of data over the LoRaWAN network. The function handles 
 *   transmission errors and retries based on the error count. If the error count 
 *   exceeds a threshold, it disconnects the device from the network.
 * 
 * Note:
 * The LoRaModem library must be installed and included in the project. The MKR WAN 1310 
 * board communicates using the EU868 frequency band.
 */

#include "Driver_LoRaWan.hpp"

// LoRa modem object for handling communication
LoRaModem modem;

// Indicates wheter the device is connected to the LoRaWAN network
bool connected = false;

// Tracks the number of consecutive transmission errors.
int err_count = 0;

/**
 * @brief Initializes the LoRaWAN modem and sets the frequency plan to EU868.
 * 
 * This function starts the LoRaWAN modem and ensures the device is ready to communicate 
 * over the EU868 frequency band. If the modem fails to start, the system halts in an infinite loop.
 */
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

  Serial.println("Module started");
}

/**
 * @brief Attempts to connect to the LoRaWAN network using OTAA.
 * 
 * This function tries to connect to the LoRaWAN network using the provided AppEUI, AppKey, and DevEUI credentials. 
 * If the connection is successful, it adjusts the polling interval and data rate, and resets the error counter. 
 * Otherwise, the connection remains inactive.
 */
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

/**
 * @brief Sends a message over the LoRaWAN network.
 * 
 * This function sends a message (given as a char array) of a specific size over the LoRaWAN network. 
 * If the transmission fails, it increments the error count. If more than 50 consecutive transmission errors occur, 
 * the connection is considered lost and `connected` is set to `false`.
 * 
 * @param msg The message to be sent as a char array.
 * @param size The size of the message to be sent.
 */
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
