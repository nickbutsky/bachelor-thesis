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
      printf("RH=%02d%% t=%dC\n", dht11.humidity, dht11.temperature);
      break;
    case DHT11_NO_CONNECTION:
      puts("NOT CONNECTED");
      break;
    case DHT11_CHECKSUM_ERROR:
      puts("CHECKSUM ERROR");
      break;
    }
    measureDht11 = 0;
  }

  if (photoresistorStatus == PHOTORESISTOR_STARTING) {
    photoresistorStatus = PHOTORESISTOR_RUNNNING;
    HAL_ADC_Start_IT(&hadc1);
  }

  // =============================================== ADC MEASUREMENT END
  if (photoresistorStatus == PHOTORESISTOR_DONE) {
    uint32_t voltage = (_INT_REF_V * photoresistorData * 100 + 50) / VREF_DATA / 100;
    uint32_t calibratedVoltage = voltage * (*voltageFactoryReferencePtr) / VREF_DATA;

    photoresistorStatus = PHOTORESISTOR_IDLE;

    printf("L=%ld, Vref=%ldm VOLT = %ldmv, VOLT(c) = %ldmv\n", photoresistorData, VREF_DATA, voltage,
           calibratedVoltage);
  }

  ++counter;
  if (counter >= DHT11_COUNTER_LIMIT) {
    measureDht11 = 1;
    photoresistorStatus = PHOTORESISTOR_STARTING;
    counter = 0;
  }
  HAL_Delay(1);
}