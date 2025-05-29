#pragma once

#include "main.h"

typedef struct {
  uint32_t linkId;
  const char *contentType;
} Esp8266;

void initialiseEsp8266();
Esp8266 runEsp8266();
void handleRequest(const Esp8266 *esp8266Ptr, const DHT11 *dht11Ptr, uint32_t photoresistorValue);