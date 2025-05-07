#ifndef _DHT11_H
#define _DHT11_H

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif


extern uint8_t		GoDHT11;
extern uint8_t		DHTstat;
extern int8_t		DHT_Temp;
extern uint8_t		DHT_Hum;


#define		_DHT_DATA_SIZE		5


#define MAX_TICS 		50000
#define ONE_TICS 		70

#define DHT11_IDLE 		0
#define DHT11_OK 		1
#define DHT11_NO_CONN 	2
#define DHT11_CS_ERROR 	3

uint16_t 	DHT_read_cycle(uint8_t neg_tic);
uint8_t 	DHT_read_DHT11(uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif // #ifndef _DHT11_H
