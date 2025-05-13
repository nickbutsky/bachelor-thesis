#include "iteration.h"

enum { COUNTER_UPPER_BOUND = 2000 };

uint16_t counter;

void iteration() {
  printEverything();

  switch (photoresistor.status) {
  case PHOTORESISTOR_STARTING:
    photoresistor.status = PHOTORESISTOR_RUNNING;
    HAL_ADC_Start_IT(&hadc1);
    break;
  case PHOTORESISTOR_DONE:
    photoresistor.status = PHOTORESISTOR_IDLE;
    printf("photoresistorValue == %ld/4095\n", photoresistor.lastValue);
    break;
  default:
    break;
  }

  int8_t channelNumber = runEsp8266();
  if (channelNumber != -1) {
    DHT11 dht11 = getDht11();
    handleApiRequest(channelNumber, &dht11, photoresistor.lastValue);
  }

  ++counter;
  if (counter >= COUNTER_UPPER_BOUND) {
    counter = 0;
    photoresistor.status = PHOTORESISTOR_STARTING;
  }
  HAL_Delay(1);
}