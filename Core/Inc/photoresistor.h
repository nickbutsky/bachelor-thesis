#pragma once

#include "main.h"

typedef enum {
  PHOTORESISTOR_IDLE,
  PHOTORESISTOR_STARTING,
  PHOTORESISTOR_RUNNING,
  PHOTORESISTOR_DONE
} PhotoresistorStatus;

struct {
  PhotoresistorStatus status;
  uint32_t value;
} photoresistor;
