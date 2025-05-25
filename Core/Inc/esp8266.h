#pragma once

#include "dht11.h"
#include "html.h"
#include "main.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>

typedef struct {
  uint32_t linkId;
  const char *contentType;
} Esp8266;

void initialiseEsp8266();
Esp8266 runEsp8266();
void handleRequest(const Esp8266 *esp8266Ptr, const DHT11 *dht11Ptr, uint32_t photoresistorValue);