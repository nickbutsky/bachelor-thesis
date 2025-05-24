#include "photoresistor.h"

uint32_t getPhotoresistorValue() {
  HAL_ADC_Start(&hadc1);
  const uint8_t timeout = 20;
  HAL_ADC_PollForConversion(&hadc1, timeout);
  return HAL_ADC_GetValue(&hadc1);
}
