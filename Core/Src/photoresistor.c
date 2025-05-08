#include "photoresistor.h"

uint16_t LIGHT_DATA = 0;
uint16_t VREF_DATA = 0;
uint8_t  LIGHT_stat = _LIGHT_IDLE;

// factory measured refference voltage is on this address
uint16_t *REF_volt = (uint16_t *)  0x1FFF7A2AUL; // 0x1FFF 7A2A - 0x1FFF 7A2B

// =============================== ADC INTERRUPT CALBACK IS HERE

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) //check if the interrupt comes from ACD1
    {
		LIGHT_DATA = HAL_ADC_GetValue(&hadc1);
		LIGHT_stat = _LIGHT_RUN_REF;
    }
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1) //check if the interrupt comes from ACD1
    {
    	if(LIGHT_stat == _LIGHT_RUN){
    		LIGHT_DATA = HAL_ADC_GetValue(&hadc1);
    		LIGHT_stat = _LIGHT_RUN_REF;
    		ADC_Set_Input_Vref();
    		HAL_ADC_Start_IT(&hadc1);
    	} else if(LIGHT_stat == _LIGHT_RUN_REF){
    		VREF_DATA = HAL_ADC_GetValue(&hadc1);
    		LIGHT_stat = _LIGHT_DONE;
    		ADC_Set_Input_Light();
    		HAL_ADC_Stop_IT(&hadc1);
    	}
    }
}

void ADC_Set_Input_Light(void)
{
	  ADC_ChannelConfTypeDef sConfig = {0};

	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

}

void ADC_Set_Input_Vref(void)
{
	  ADC_ChannelConfTypeDef sConfig = {0};

	  sConfig.Channel = ADC_CHANNEL_VREFINT;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
