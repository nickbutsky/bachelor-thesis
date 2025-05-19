#include "iteration.h"

uint16_t counter;

void iteration() {
  printEverything();

  int8_t linkId = runEsp8266();
  if (linkId != -1) {
    DHT11 dht11 = getDht11();
    handleApiRequest(linkId, &dht11, getLastPhotoresistorValue());
  }

  ++counter;
  enum { COUNTER_UPPER_BOUND = 50 };
  if (counter >= COUNTER_UPPER_BOUND) {
    runPhotoresistorAsync();
  }

  HAL_Delay(1);
}