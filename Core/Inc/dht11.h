#pragma once

#include "main.h"

typedef enum { DHT11_OK, DHT11_NO_CONNECTION, DHT11_CHECKSUM_ERROR } DHT11Status;

typedef struct {
  DHT11Status status;
  int8_t temperature;
  uint8_t humidity;
} DHT11;

DHT11 getDht11();
