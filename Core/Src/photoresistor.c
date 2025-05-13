#include "photoresistor.h"

uint32_t lastValue;

uint32_t getLastPhotoresistorValue() { return lastValue; }

void updatePhotoresistorValue() { HAL_ADC_Start_IT(&hadc1); }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    lastValue = HAL_ADC_GetValue(&hadc1);
  }
}
