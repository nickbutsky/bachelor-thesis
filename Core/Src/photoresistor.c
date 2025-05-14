#include "photoresistor.h"

uint32_t value;

uint32_t getLastPhotoresistorValue() { return value; }

void runPhotoresistorAsync() { HAL_ADC_Start_IT(&hadc1); }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    value = HAL_ADC_GetValue(&hadc1);
  }
}
