#include "photoresistor.h"

uint32_t photoresistorData;
uint8_t photoresistorStatus = PHOTORESISTOR_IDLE;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (!(hadc->Instance == ADC1 && photoresistorStatus == PHOTORESISTOR_RUNNNING)) {
    return;
  }
  photoresistorData = HAL_ADC_GetValue(&hadc1);
  photoresistorStatus = PHOTORESISTOR_DONE;
}
