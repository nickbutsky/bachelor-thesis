#include "iteration.h"

void iteration() {
  printEverything();

  int8_t linkId = runEsp8266();
  if (linkId != -1) {
    DHT11 dht11 = getDht11();
    handleApiRequest(linkId, &dht11, getPhotoresistorValue());
  }

  HAL_Delay(1);
}