#include "iteration.h"

enum { DHT11_COUNTER_LIMIT = 2000 };

uint16_t counter;
uint8_t measureDht11 = 1;

void iteration() {
  printEverything();
  if (measureDht11) {
    DHT11 dht11 = getDht11();
    switch (dht11.status) {
    case DHT11_OK:
      printf("RH=%02d%% t=%dC\r\n", dht11.humidity, dht11.temperature);
      break;
    case DHT11_NO_CONNECTION:
      printf("NOT CONNECTED\r\n");
      break;
    case DHT11_CHECKSUM_ERROR:
      printf("CHECKSUM ERROR\r\n");
      break;
    }
    measureDht11 = 0;
  }
  ++counter;
  if (counter >= DHT11_COUNTER_LIMIT) {
    measureDht11 = 1;
    counter = 0;
  }
  HAL_Delay(1);
}