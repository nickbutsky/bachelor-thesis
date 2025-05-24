#pragma once

#include "dht11.h"
#include "html.h"
#include "main.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>

void initialiseEsp8266();
int8_t runEsp8266();
void handleApiRequest(uint8_t channelNumber, const DHT11 *dht11Ptr, uint32_t photoresistorValue);