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

  if (LIGHT_stat == _LIGHT_START) {
    LIGHT_stat = _LIGHT_RUN;
    HAL_ADC_Start_IT(&hadc1);
  }

  // =============================================== ADC MEASUREMENT END
  if (LIGHT_stat == _LIGHT_DONE) {
    uint32_t VOLT = ((uint32_t)_INT_REF_V * LIGHT_DATA * 100 + 50) / VREF_DATA / 100;
    uint32_t VOLT_C = ((((uint32_t)_INT_REF_V * LIGHT_DATA * 100 + 50) / VREF_DATA / 100) * (*REF_volt)) / VREF_DATA;

    LIGHT_stat = _LIGHT_IDLE;

    printf("L=%d, Vref=%dm VOLT = %dmv, VOLT(c) = %dmv\r\n", LIGHT_DATA, VREF_DATA, (uint16_t)VOLT, (uint16_t)VOLT_C);
  }

  ++counter;
  if (counter >= DHT11_COUNTER_LIMIT) {
    measureDht11 = 1;
    counter = 0;
  }
  HAL_Delay(1);
}