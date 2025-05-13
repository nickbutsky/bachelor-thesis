#include "iteration.h"

enum { COUNTER_UPPER_BOUND = 50 };

uint16_t counter;

void iteration() {
  printEverything();

  int8_t channelNumber = runEsp8266();
  if (channelNumber != -1) {
    DHT11 dht11 = getDht11();
    handleApiRequest(channelNumber, &dht11, getLastPhotoresistorValue());
  }

  ++counter;
  if (counter >= COUNTER_UPPER_BOUND) {
    updatePhotoresistorValue();
  }

  HAL_Delay(1);
}