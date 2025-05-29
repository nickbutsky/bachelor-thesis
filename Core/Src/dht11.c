#include "dht11.h"

enum { DATA_LENGTH = 40, MAX_TICK_NUMBER = 1000 };

static inline uint8_t readData(uint16_t *data) {
  enum { RESET_DELAY = 500, SET_DELAY = 20 };
  uint16_t tickNumber = 0;
  const uint8_t five = 5;
  HAL_Delay(RESET_DELAY);
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
  HAL_Delay(SET_DELAY);
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
  for (uint32_t i = 0; i < (DATA_LENGTH * 2) + 4; ++i) {
    tickNumber = 0;
    GPIO_PinState waitValue = i % 2 ? GPIO_PIN_SET : GPIO_PIN_RESET;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == waitValue) {
      ++tickNumber;
      if (tickNumber >= MAX_TICK_NUMBER) {
        HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
        return 1;
      }
    }
    if (waitValue == GPIO_PIN_SET && i >= five) {
      data[(i - five) / 2] = tickNumber;
    }
  }
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
  return 0;
}

DHT11 getDht11() {
  uint16_t data[DATA_LENGTH] = {0};
  if (readData(data)) {
    return (DHT11){DHT11_NO_CONNECTION};
  }

  uint16_t maxTickNumber = 0;
  uint16_t minTickNumber = MAX_TICK_NUMBER;
  for (uint32_t i = 0; i < DATA_LENGTH; ++i) {
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

  enum { BUFFER_LENGTH = 5 };
  uint8_t buffer[BUFFER_LENGTH] = {0};
  uint16_t maxTickNumberMinTickNumberMean = (maxTickNumber + minTickNumber) / 2;
  for (uint32_t i = 0; i < DATA_LENGTH; ++i) {
    const uint8_t eight = 8;
    uint8_t bufferIndex = i / eight;
    buffer[bufferIndex] *= 2;
    if (data[i] > maxTickNumberMinTickNumberMean) {
      buffer[bufferIndex] |= (uint8_t)1;
    }
  }

  uint8_t checkSum = 0;
  for (uint32_t i = 0; i < BUFFER_LENGTH - 1; ++i) {
    checkSum += buffer[i];
  }
  if (checkSum != buffer[BUFFER_LENGTH - 1]) {
    return (DHT11){DHT11_CHECKSUM_ERROR};
  }

  return (DHT11){DHT11_OK, (int8_t)buffer[2], buffer[0]};
}
