/*
 * File: Driver_Credentials.cpp
 * 
 * Description:
 * This source file implements the Driver Credentials functionality 
 * for managing device credentials in a LoRaWAN modem. It includes 
 * the definitions of functions for initializing, reading, and writing 
 * credentials such as devEUI, appEUI, and appKey. 
 * 
 * The credentials are set and stored as strings via AT commands, 
 * allowing for easy configuration by the user.
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

#include "Driver_Credentials.hpp"

// Credentials stored as strings, configured via AT commands.
String appEui = "", appKey = "", devEui = ""; 

// Indicates the configuration state of the credentials.
bool configuration = false;

/**
 * @brief Initializes the credentials by waiting for AT commands from the user.
 *
 * This function prepares the system to receive AT commands from the Serial interface.
 * It listens for input until a valid command is received (indicated by a newline character).
 * Once the credentials are initied, it updates the NVM with specific values.
 *
 * It performs the following steps:
 * - Reserves memory for input commands.
 * - Waits for user input on the Serial interface.
 * - Processes the received command when a complete string is detected.
 * - Updates the NVM with a status and a magic number.
 * - Sends a command to the LoRa modem to lock the AppKey access.
 *
 * @note The function will block until the 'configuration' flag is set to true,
 * indicating that the credential initialization process is complete.
 *
 * @see processCommand() for details on how commands are processed.
 * @see writeNVM() for details on writing to the Non-Volatile Memory.
 */
void init_Credentials()
{
  Serial.println("Ready to receive AT commands. Type AT? for assistance");
  String inputString = ""; 
  inputString.reserve(200);    
  bool stringComplete = false;  

  while(!configuration)
  {
    while (Serial.available()) 
    {
      char inChar = (char)Serial.read();
      inputString += inChar;
      if (inChar == '\n') 
      {
        stringComplete = true;
      }
    }

    if(stringComplete == true)
    {
      processCommand(inputString);

      inputString = "";
      stringComplete = false;
    }
    delay(100);
  }
  writeNVM(1,1);
  writeNVM(2,MAGICNUMBER+1);
  SerialLoRa.println("AT$APKACCESS");
}

/**
 * @brief Checks if the credentials have already been initialized.
 *
 * This function reads values from NVM to determine
 * if the credentials have been set up correctly. It checks the following:
 * - The magic number stored at address 0 matches the expected MAGICNUMBER.
 * - The state stored at address 1 is equal to 1.
 * - The checksum, stored at address 2, is valid, which is calculated by
 *   summing the magic number and the state.
 *
 * @return true if the credentials are already initialized and valid,
 *         false otherwise.
 *
 * @see readNVM() for details on reading to the Non-Volatile Memory.
 */
bool credentialsAlreadyInit()
{
  uint8_t magicNumber = readNVM(0);
  uint8_t state = readNVM(1);
  uint8_t checksum = readNVM(2);
  bool init = true;
  if(magicNumber != MAGICNUMBER || state != 1 || magicNumber + state != checksum)
  {
    init = false;
  }
  return init;
}

/**
 * @brief Processes an incoming AT command string.
 *
 * This function interprets and executes various AT commands received
 * via the serial interface. The following commands are supported:
 * - "AT?\n": Displays a list of available commands.
 * - "AT+D=<devEUI>": Configures the device EUI.
 * - "AT+A=<appEUI>": Configures the application EUI.
 * - "AT+K=<appKey>": Configures the application key.
 * - "AT+S\n": Saves the configured credentials.
 *
 * The function also validates the input for devEUI, appEUI, and appKey
 * using their respective validation functions. If a command is invalid or
 * incomplete, an appropriate error message is displayed.
 *
 * @param command The AT command string to be processed.
 *
 * @note The function modifies the global variables `devEui`, `appEui`,
 *       and `appKey` based on the received commands. The `configuration`
 *       variable is set to true upon successful configuration of all
 *       credentials.
 */
void processCommand(String command)
{
  if (command == "AT?\n")
  {
    Serial.println("");
    Serial.println("Commands available : ");
    Serial.println("AT+D=<devEUI> : Configure the devEUI");
    Serial.println("AT+A=<appEUI> : Configure the appEUI");
    Serial.println("AT+K=<appKey> : Configure the appKey");
    Serial.println("AT+S : Save and protecte credentials");
  }

  else if (command.startsWith("AT+D="))
  {
    command.remove(0,5);
    
    if(isDevEUI(command))
    {
      devEui = command;
      Serial.println("DevEUI OK");
    }
    else
    {
      Serial.println("DevEUI incorrect, please try again");
    }
  }

  else if (command.startsWith("AT+A"))
  {
    command.remove(0,5);

    if(isAppEUI(command))
    {
      appEui = command;
      Serial.println("AppEUI OK");
    }
    else
    {
      Serial.println("AppEUI incorrect, please try again");
    }
  }
  else if(command.startsWith("AT+K"))
  {
    command.remove(0,5);

    if(isAppKey(command))
    {
      appKey = command;
      Serial.println("AppKey OK");
    }
    else
    {
      Serial.println("AppKey incorrect, please try again");
    }
  }
  else if(command == "AT+S\n")
  {
    if(appEui!="" && appEui!="" && appKey!="")
    {
      Serial.println("Configuration of the credentials finished");
      configuration = true;
      
    }
    else
    {
      Serial.println("You have to configure all the credentials");
    }
  }
  else
  {
    Serial.println("Invalid command, type AT? for assistance");
  }
}

/**
 * @brief Validates a given credential string.
 *
 * This function checks if the provided credential string meets the 
 * specified length and consists solely of valid hexadecimal characters 
 * (0-9, A-F, a-f). The length check considers the inclusion of a 
 * newline character (`\n`) at the end of the string.
 *
 * @param credential The credential string to be validated.
 * @param size The expected size of the credential, excluding the newline character.
 *
 * @return true if the credential is valid (correct length and valid characters),
 *         false otherwise.
 *
 * @note The function assumes that a valid credential string ends with a newline 
 *       character. Therefore, the `size` parameter should be specified as 
 *       the length of the credential excluding the newline.
 */
bool isCredential(String credential, int size)
{
  if(credential.length() != size + 1)  
  {
    return false;
  }
  
  int k = 0;
  while (k < size && ((credential[k] >= '0' && credential[k] <= '9') || (credential[k] >= 'A' && credential[k] <= 'F') || (credential[k] >= 'a' && credential[k] <= 'f')))
  {
    k ++;
  }
  if(k != size)
  {
    return false;
  }
  else
  {
    return true;
  }
}

/**
 * @brief Validates a Device EUI string.
 *
 * This function checks if the provided Device EUI string is valid by 
 * verifying that it is a hexadecimal string of 16 characters.
 *
 * @param devEUI The Device EUI string to be validated.
 *
 * @return true if the Device EUI is valid, false otherwise.
 */
bool isDevEUI(String devEUI)
{
  return isCredential(devEUI, 16);
}

/**
 * @brief Validates an Application EUI string.
 *
 * This function checks if the provided Application EUI string is valid by 
 * verifying that it is a hexadecimal string of 16 characters.
 *
 * @param appEUI The Application EUI string to be validated.
 *
 * @return true if the Application EUI is valid, false otherwise.
 */
bool isAppEUI(String appEUI)
{
  return isCredential(appEUI, 16);
}

/**
 * @brief Validates an Application Key string.
 *
 * This function checks if the provided Application Key string is valid by 
 * verifying that it is a hexadecimal string of 32 characters.
 *
 * @param appKey The Application Key string to be validated.
 *
 * @return true if the Application Key is valid, false otherwise.
 */
bool isAppKey(String appKey)
{
  return isCredential(appKey, 32);
}

/**
 * @brief Reads a value from Non-Volatile Memory.
 *
 * This function sends a command to read a value from the NVM at the specified address 
 * using the SerialLoRa interface. It processes the response to extract the value. 
 * If the response indicates an error, it returns 255.
 *
 * @param address The address in NVM from which to read the value.
 * 
 * @return The value read from NVM as an 8-bit unsigned integer. 
 *         Returns -1 in case of an error or if the response contains "+ERR".
 */
uint8_t readNVM(uint8_t address)
{
  SerialLoRa.println("AT$NVM " + String(address));
  delay(100);
  String response = SerialLoRa.readString();

  if(response.indexOf("+ERR") == -1)
  {
    int startIndex = response.indexOf("=");
    int endIndex = response.indexOf('\n', startIndex);
    response = response.substring(startIndex  + 1, endIndex);
  }
  else
  { 
    response = "255";
  }
  return response.toInt();
}

/**
 * @brief Writes a value to the Non-Volatile Memory at a specified address.
 *
 * This function sends a command to the LoRa module to write a byte value to a specific
 * address in the Non-Volatile Memory. It waits for a response to confirm if the write
 * operation was successful.
 *
 * @param address The address in the NVM where the value will be written. It should be a valid
 *        address within the range supported by the device.
 * @param value The byte value to write to the specified address in the NVM.
 *
 * @return Returns true if the write operation was successful (indicated by a "+OK" response),
 *         false otherwise.
 */
bool writeNVM(uint8_t address, uint8_t value)
{
  SerialLoRa.println("AT$NVM " + String(address) + "," + String(value));
  delay(100);
  String response = SerialLoRa.readString();
  bool success = false;

  if(response.indexOf("+OK") != -1)
  {
    success = true;
  }
  return success;
}
