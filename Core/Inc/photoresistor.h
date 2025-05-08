#ifndef _ADC_H
#define _ADC_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif


// ========================================= Global ADC Handler
extern ADC_HandleTypeDef hadc1;

// ========================================= Internal refference voltage
extern uint16_t 		*REF_volt; 	// @0x1FFF7A2A - 0x1FFF7A2B, for 1.210V
#define _INT_REF_V		1210		// in mV

// ========================================= ADC defs
// ADC PA1
#define	_LIGHT_IDLE		0
#define	_LIGHT_START	1
#define	_LIGHT_RUN		2
#define	_LIGHT_RUN_REF	3
#define	_LIGHT_DONE		4

// ========================================= ADC vars
extern uint8_t  LIGHT_stat;
extern uint16_t LIGHT_DATA;
extern uint16_t VREF_DATA;

// ========================================= ADC functions
void 	ADC_Set_Input_Light(void);
void 	ADC_Set_Input_Vref(void);


#ifdef __cplusplus
}
#endif

#endif // #ifndef _ADC_H
