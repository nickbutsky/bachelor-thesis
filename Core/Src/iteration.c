#include "iteration.h"

void iteration() {
  printEverything();

  Esp8266 esp8266 = runEsp8266();
  if (esp8266.linkId != -1) {
    DHT11 dht11 = getDht11();
    handleRequest(&esp8266, &dht11, getPhotoresistorValue());
  }

  HAL_Delay(1);
}