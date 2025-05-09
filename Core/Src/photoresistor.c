#include "photoresistor.h"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (!(hadc->Instance == ADC1 && photoresistor.status == PHOTORESISTOR_RUNNING)) {
    return;
  }
  photoresistor.value = HAL_ADC_GetValue(&hadc1);
  photoresistor.status = PHOTORESISTOR_DONE;
}
