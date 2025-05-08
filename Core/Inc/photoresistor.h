#pragma once

#include "main.h"

typedef enum {
  PHOTORESISTOR_IDLE,
  PHOTORESISTOR_STARTING,
  PHOTORESISTOR_RUNNING,
  PHOTORESISTOR_DONE
} PhotoresistorStatus;

extern PhotoresistorStatus photoresistorStatus;
extern uint32_t photoresistorValue;
