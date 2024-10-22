/*
 * File: Driver_SHT31.cpp
 * 
 * Description:
 * This source file implements the driver for the SHT31 temperature 
 * and humidity sensor using the Adafruit SHT31 library. It includes 
 * the initialization function for setting up communication with the 
 * SHT31 sensor.
 * 
 * Functions:
 * - init_SHT31: Initializes the SHT31 sensor by checking if it is properly 
 *   connected to the I2C bus. If the sensor is not found, it prints an error 
 *   message to the Serial monitor and enters an infinite loop.
 * 
 * Note:
 * The Adafruit_SHT31 library must be installed and included in the project. 
 * The sensor communicates via I2C at address 0x44.
 */

#include "Driver_SHT31.hpp"

// Creates an instance of the Adafruit_SHT31 sensor object.
Adafruit_SHT31 sht31 = Adafruit_SHT31();

/**
 * @brief Initializes the SHT31 temperature and humidity sensor.
 * 
 * This function initializes the SHT31 sensor by attempting to establish 
 * communication over the I2C bus. It checks if the sensor is connected 
 * at the address 0x44. If the sensor is not found, an error message is 
 * printed to the Serial monitor and the system enters an infinite loop.
 */
void init_SHT31()
{
  if (! sht31.begin(0x44)) 
  {   
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
}
