#ifndef HPP__DRIVERLORAWAN__HPP
#define HPP__DRIVERLORAWAN__HPP

#include <MKRWAN.h>
#include "Secret.hpp"

extern String appEui;
extern String appKey;
extern String devEui;

extern LoRaModem modem;
extern bool connected;
extern int err_count;

void init_LoRaWan();
void connect();
void send(char msg[], int size);

#endif