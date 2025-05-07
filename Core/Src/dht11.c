#include "DHT11.h"


uint8_t		GoDHT11 = 0;
uint8_t		DHTstat = 0;
int8_t		DHT_Temp = 0;
uint8_t		DHT_Hum = 0;




uint16_t DHT_read_cycle(uint8_t neg_tic)
{
  uint16_t cnt_tics = 0;

  if (neg_tic) {
    while ( !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) && (cnt_tics < MAX_TICS)){
      cnt_tics++;
    }
  } else {
    while ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) && (cnt_tics < MAX_TICS)){
      cnt_tics++;
    }
  }
  return cnt_tics;
}

uint8_t DHT_read_DHT11(uint8_t *buf)
{
	uint16_t 	dt[42];
	uint16_t 	cnt;
	uint8_t 	i;
	uint8_t 	check_sum;

	uint16_t	min_tic = MAX_TICS;
	uint16_t	max_tic = 0;

	uint32_t	mean = 0;

	//reset DHT11
	HAL_Delay(500);
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
 	HAL_Delay(20);
 	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

    //start reading
 	cnt = 0;
	for(i = 0; i < 83 && cnt < MAX_TICS; i++){
		if (i & 1){
			cnt = DHT_read_cycle(1);
		}
		else {
			cnt = DHT_read_cycle(0);
			dt[i/2]= cnt;
		}
	}

 	//release line
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

	printf("RAW: ");
 	for(i = 2; i < 42; i++){
		printf(" %d", dt[i]);
	}
	printf("\r\n");

	if (cnt >= MAX_TICS) return DHTstat = DHT11_NO_CONN;

	//find min-max
 	for(i = 2; i < 42; i++){
 		mean += dt[i];
 		if(dt[i] && (dt[i] < MAX_TICS)){
 			if(dt[i] > max_tic) max_tic = dt[i];
 			if(dt[i] < min_tic) min_tic = dt[i];
 		}
 	}
 	mean /= 40;

 	printf("RAW DIVIDER: min %d max %d\r\n", min_tic, max_tic);

 	if(max_tic < min_tic) return DHTstat = DHT11_NO_CONN;
 	max_tic = (max_tic + min_tic) / 2;

 	printf("RAW DIVIDER: minmax %d mean %ld\r\n", max_tic, mean);

	//convert data
 	for(i = 2; i < 42; i++){
		(*buf) <<= 1;
		if (dt[i] > max_tic) {
			(*buf) |= 1;
 		}
		if (!((i-1) % 8) && (i > 2)) {
			buf++;
		}
 	}

	//calculate checksum
	buf -= 5;
	check_sum = 0;

	printf("RAW DAT: ");
	for(i = 0; i < 5; i++){
		printf(" %d", buf[i]);
	}
	printf("\r\n");

 	for(i = 0; i < 4; i++){
		check_sum += *buf;
		buf++;
	}
 	if(*buf != check_sum){
 		DHT_Hum = DHT_Temp = 0;
 		DHTstat = DHT11_CS_ERROR;
 	} else {
 		buf -= 4;
 		DHT_Hum = buf[0];
 		DHT_Temp = buf[2];
 		DHTstat = DHT11_OK;
 	}

	return DHTstat;
}







