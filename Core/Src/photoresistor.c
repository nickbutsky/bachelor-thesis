#include "photoresistor.h"

uint32_t photoresistorData;
uint32_t VREF_DATA;
uint8_t photoresistorStatus = PHOTORESISTOR_IDLE;

// factory measured reference voltage is on this address
const uint16_t *const voltageFactoryReferencePtr = (uint16_t *)0x1FFF7A2AUL; // 0x1FFF 7A2A - 0x1FFF 7A2B

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    photoresistorData = HAL_ADC_GetValue(&hadc1);
    photoresistorStatus = PHOTORESISTOR_RUNNING_REF;
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance != ADC1) {
    return;
  }
  if (photoresistorStatus == PHOTORESISTOR_RUNNNING) {
    photoresistorData = HAL_ADC_GetValue(&hadc1);
    photoresistorStatus = PHOTORESISTOR_RUNNING_REF;
    if (HAL_ADC_ConfigChannel(&hadc1, &(ADC_ChannelConfTypeDef){ADC_CHANNEL_VREFINT, 1, ADC_SAMPLETIME_28CYCLES}) !=
        HAL_OK) {
      Error_Handler();
    }
    HAL_ADC_Start_IT(&hadc1);
  } else if (photoresistorStatus == PHOTORESISTOR_RUNNING_REF) {
    VREF_DATA = HAL_ADC_GetValue(&hadc1);
    photoresistorStatus = PHOTORESISTOR_DONE;
    if (HAL_ADC_ConfigChannel(&hadc1, &(ADC_ChannelConfTypeDef){ADC_CHANNEL_1, 1, ADC_SAMPLETIME_28CYCLES}) != HAL_OK) {
      Error_Handler();
    }
    HAL_ADC_Stop_IT(&hadc1);
  }
}
