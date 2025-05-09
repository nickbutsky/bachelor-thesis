#include "iteration.h"

enum { COUNTER_UPPER_BOUND = 2000 };

uint16_t counter;
uint8_t measureDht11 = 1;

void iteration() {
  printEverything();

  if (measureDht11) {
    measureDht11 = 0;
    DHT11 dht11 = getDht11();
    switch (dht11.status) {
    case DHT11_OK:
      printf("humidity == %02d%%\ntemperature == %dC\n", dht11.humidity, dht11.temperature);
      break;
    case DHT11_NO_CONNECTION:
      puts("NOT CONNECTED");
      break;
    case DHT11_CHECKSUM_ERROR:
      puts("CHECKSUM ERROR");
      break;
    }
  }

  switch (photoresistorStatus) {
  case PHOTORESISTOR_STARTING:
    photoresistorStatus = PHOTORESISTOR_RUNNING;
    HAL_ADC_Start_IT(&hadc1);
    break;
  case PHOTORESISTOR_DONE:
    photoresistorStatus = PHOTORESISTOR_IDLE;
    printf("photoresistorValue == %ld/4095\n", photoresistorValue);
    break;
  default:
    break;
  }

  ++counter;
  if (counter >= COUNTER_UPPER_BOUND) {
    counter = 0;
    measureDht11 = 1;
    photoresistorStatus = PHOTORESISTOR_STARTING;
  }
  HAL_Delay(1);
}