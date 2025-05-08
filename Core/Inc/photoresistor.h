#pragma once

#include "main.h"

enum {
  PHOTORESISTOR_IDLE,
  PHOTORESISTOR_STARTING,
  PHOTORESISTOR_RUNNNING,
  PHOTORESISTOR_RUNNING_REF,
  PHOTORESISTOR_DONE
};

extern uint8_t photoresistorStatus;
extern uint32_t photoresistorData;
