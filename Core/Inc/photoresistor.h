#pragma once

#include "main.h"

// ========================================= Internal reference voltage
extern const uint16_t *const voltageFactoryReferencePtr; // @0x1FFF7A2A - 0x1FFF7A2B, for 1.210V
#define _INT_REF_V 1210                                  // in mV

enum {
  PHOTORESISTOR_IDLE,
  PHOTORESISTOR_STARTING,
  PHOTORESISTOR_RUNNNING,
  PHOTORESISTOR_RUNNING_REF,
  PHOTORESISTOR_DONE
};

// ========================================= ADC vars
extern uint8_t photoresistorStatus;
extern uint32_t photoresistorData;
extern uint32_t VREF_DATA;
