/*
 * File: Driver_SHT31.hpp
 * 
 * Description:
 * This header file contains the implementation of the driver SHT31 temperature 
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

#ifndef HPP__DRIVERSHT31__HPP
#define HPP__DRIVERSHT31__HPP

#include <Wire.h>
#include <Adafruit_SHT31.h>

extern Adafruit_SHT31 sht31;

void init_SHT31();

#endif