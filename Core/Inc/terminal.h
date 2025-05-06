#pragma once

#include "main.h"

enum { RX_BUFFER_SIZE = 32 };

extern char rxBuffer[RX_BUFFER_SIZE];
extern uint8_t rxPutIndex;
extern uint8_t rxGetIndex;
extern volatile char rxCharacter;

#define isRxAvailable() (rxPutIndex - rxGetIndex)

void initialiseRx();
char readRx();
