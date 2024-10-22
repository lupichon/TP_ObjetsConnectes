/*
 * File: Driver_Credentials.hpp
 * Author: [Your Name]
 * Date: [Date]
 * 
 * Description:
 * This header file contains the implementation of the Driver Credentials functionality 
 * for managing device credentials in a LoRaWAN modem. It provides functions for 
 * initializing, reading, and writing credentials such as devEUI, appEUI, and appKey. 
 * The credentials are set and stored as strings via AT commands, allowing for easy 
 * configuration by the user.
 * 
 * Functions:
 * - init_Credentials: Initializes the credential input process.
 * - credentialsAlreadyInit: Checks if the credentials have already been initialized.
 * - processCommand: Processes incoming AT commands related to credentials.
 * - isCredential: Validates the format of the provided credentials.
 * - isDevEUI: Validates the devEUI format.
 * - isAppEUI: Validates the appEUI format.
 * - isAppKey: Validates the appKey format.
 * - readNVM: Reads a value from Non-Volatile Memory (NVM).
 * - writeNVM: Writes a value to Non-Volatile Memory (NVM).
 */


#ifndef HPP__DRIVERCREDENTIAL__HPP
#define HPP__DRIVERCREDENTIAL__HPP

#include <Arduino.h>
#include "Secret.hpp"

#define MAGICNUMBER 92

extern String appEui;
extern String appKey;
extern String devEui;

void init_Credentials();
bool credentialsAlreadyInit();
void processCommand(String command);
bool isCredential(String credential, int size);
bool isDevEUI(String devEUI);
bool isAppEUI(String appEUI);
bool isAppKey(String appKey);
uint8_t readNVM(uint8_t address);
bool writeNVM(uint8_t address, uint8_t value);

#endif