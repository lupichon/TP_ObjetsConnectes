/*
 * File: Driver_LoRaWan.hpp
 * 
 * Description:
 * This header file contains the implementation of the driver for managing 
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

#ifndef HPP__DRIVERLORAWAN__HPP
#define HPP__DRIVERLORAWAN__HPP

#include <MKRWAN.h>
#include "Driver_Credentials.hpp"

extern LoRaModem modem;
extern bool connected;
extern int err_count;

void init_LoRaWan();
void connect();
void send(char msg[], int size);

#endif