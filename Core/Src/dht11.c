#include "dht11.h"

enum { RESET_DELAY = 500, SET_DELAY = 20, DATA_LENGTH = 42, BUFFER_LENGTH = 5, MAX_TICK_NUMBER = 50000 };

static inline uint8_t readData(uint16_t *data) {
  uint8_t iterator = 0;
  const uint8_t maxResponseIterator = 200;
  uint8_t upperBound = (DATA_LENGTH * 2) - 1;
  uint8_t iteratorOdd = 0;
  uint16_t tickNumber = 0;
  HAL_Delay(RESET_DELAY);
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
  HAL_Delay(SET_DELAY);
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
  for (iterator = 0; iterator < maxResponseIterator; ++iterator) {
    if (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == GPIO_PIN_RESET) {
      break;
    }
  }
  if (iterator >= maxResponseIterator) {
    return 1;
  }
  for (iterator = 0; iterator < upperBound; ++iterator) {
    iteratorOdd = iterator & (uint8_t)1;
    tickNumber = 0;
    if (iteratorOdd) {
      while (!HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && tickNumber < MAX_TICK_NUMBER) {
        ++tickNumber;
      }
    } else {
      while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) && tickNumber < MAX_TICK_NUMBER) {
        ++tickNumber;
      }
      data[iterator / 2] = tickNumber;
    }
  }
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
  return tickNumber >= MAX_TICK_NUMBER ? 1 : 0;
}

DHT11 getDht11() {
  uint16_t data[DATA_LENGTH] = {0};
  if (readData(data)) {
    return (DHT11){DHT11_NO_CONNECTION};
  }

  uint16_t maxTickNumber = 0;
  uint16_t minTickNumber = MAX_TICK_NUMBER;
  for (uint8_t i = 2; i < (uint8_t)DATA_LENGTH; ++i) {
    if (!data[i] || data[i] >= MAX_TICK_NUMBER) {
      continue;
    }
    if (data[i] > maxTickNumber) {
      maxTickNumber = data[i];
    }
    if (data[i] < minTickNumber) {
      minTickNumber = data[i];
    }
  }
  if (maxTickNumber < minTickNumber) {
    return (DHT11){DHT11_NO_CONNECTION};
  }

  uint8_t buffer[BUFFER_LENGTH] = {0};
  uint16_t maxTickNumberMinTickNumberMean = (maxTickNumber + minTickNumber) / 2;
  for (uint8_t i = 2; i < (uint8_t)DATA_LENGTH; ++i) {
    const uint8_t eight = 8;
    uint8_t bufferIndex = (i - 1) / eight;
    if (!((i - 1) % eight)) {
      --bufferIndex;
    }
    buffer[bufferIndex] *= 2;
    if (data[i] > maxTickNumberMinTickNumberMean) {
      buffer[bufferIndex] |= (uint8_t)1;
    }
  }

  uint8_t checkSum = 0;
  for (uint8_t i = 0; i < (uint8_t)BUFFER_LENGTH - 1; ++i) {
    checkSum += buffer[i];
  }
  if (checkSum != buffer[BUFFER_LENGTH - 1]) {
    return (DHT11){DHT11_CHECKSUM_ERROR};
    ;
  }
  return (DHT11){DHT11_OK, (int8_t)buffer[2], buffer[0]};
}
