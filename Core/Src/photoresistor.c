#include "photoresistor.h"

PhotoresistorStatus photoresistorStatus = PHOTORESISTOR_IDLE;
uint32_t photoresistorValue;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (!(hadc->Instance == ADC1 && photoresistorStatus == PHOTORESISTOR_RUNNING)) {
    return;
  }
  photoresistorValue = HAL_ADC_GetValue(&hadc1);
  photoresistorStatus = PHOTORESISTOR_DONE;
}
