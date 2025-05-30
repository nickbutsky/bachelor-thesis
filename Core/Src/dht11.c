#include "dht11.h"

DHT11 getDht11() {
  const uint16_t resetDelay = 500;
  const uint8_t setDelay = 20;
  uint16_t tickNumber = 0;
  enum { DATA_LENGTH = 40 };
  const uint16_t maxTickNumber = 1000;
  uint16_t data[DATA_LENGTH] = {0};
  HAL_Delay(resetDelay);
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);
  HAL_Delay(setDelay);
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
  for (uint32_t i = 0; i < (DATA_LENGTH * 2) + 4; ++i) {
    tickNumber = 0;
    GPIO_PinState waitValue = i % 2 ? GPIO_PIN_SET : GPIO_PIN_RESET;
    while (HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) == waitValue) {
      ++tickNumber;
      if (tickNumber >= maxTickNumber) {
        HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);
        return (DHT11){DHT11_NO_CONNECTION};
      }
    }
    if (waitValue == GPIO_PIN_SET && i >= 4) {
      data[(i - 4) / 2] = tickNumber;
    }
  }
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET);

  uint16_t maxFoundTickNumber = 0;
  uint16_t minFoundTickNumber = maxTickNumber;
  for (uint32_t i = 0; i < DATA_LENGTH; ++i) {
    if (data[i] > maxFoundTickNumber) {
      maxFoundTickNumber = data[i];
    }
    if (data[i] < minFoundTickNumber) {
      minFoundTickNumber = data[i];
    }
  }
  if (maxFoundTickNumber < minFoundTickNumber) {
    return (DHT11){DHT11_NO_CONNECTION};
  }

  enum { BUFFER_LENGTH = 5 };
  uint8_t buffer[BUFFER_LENGTH] = {0};
  uint16_t extremeTickNumberMean = (maxFoundTickNumber + minFoundTickNumber) / 2;
  for (uint32_t i = 0; i < DATA_LENGTH; ++i) {
    const uint8_t eight = 8;
    uint8_t bufferIndex = i / eight;
    buffer[bufferIndex] *= 2;
    if (data[i] > extremeTickNumberMean) {
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
