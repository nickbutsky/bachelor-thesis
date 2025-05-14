#include "esp8266.h"

enum {
  GET_DATA_MAX_LENGTH = 512,
  SEND_DATA_MAX_LENGTH = 2048,
  COMMAND_MAX_LENGTH = 64,
  HEADERS_MAX_LENGTH = 128,
  API_RESPONSE_MAX_LENGTH = 128
};

static inline void getData(char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Receive(&huart1, (uint8_t *)data, GET_DATA_MAX_LENGTH, timeout);
}

static inline void sendData(const char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Transmit(&huart1, (uint8_t *)data, strlen(data), timeout);
}

static inline void sendCommandUntilInData(const char *command, uint16_t delay, const char *substring) {
  while (1) {
    char response[GET_DATA_MAX_LENGTH] = {0};
    sendData(command);
    getData(response);
    if (strstr(response, substring)) {
      break;
    }
    HAL_Delay(delay);
  }
}

void initialiseEsp8266() {
  const char *commands[] = {"AT+RST\r\n",
                            "AT+CWMODE_CUR=2\r\n",
                            "AT+CWSAP_CUR=\"STM32\",\"12345678\",1,3,4,0\r\n",
                            "AT+CIPAP_CUR=\"192.168.51.1\"\r\n",
                            "AT+CIPMUX=1\r\n",
                            "AT+CIPSERVER=1,80\r\n"};
  const uint16_t delay = 500;
  for (uint32_t i = 0; i < lengthof(commands); ++i) {
    sendCommandUntilInData(commands[i], delay, "OK");
  }
}

static inline int8_t getChannelNumber(const char *data) {
  enum { SUBSTRING_LENGTH = 7 };
  const uint8_t channelNumber = 8;
  char substring[SUBSTRING_LENGTH] = {0};
  for (int8_t i = 0; i < channelNumber; ++i) {
    (void)sprintf(substring, "+IPD,%d", i);
    if (strstr(data, substring)) {
      return i;
    }
  }
  return -1;
}

static inline void closeChannel(uint8_t channelNumber) {
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPCLOSE=%d\r\n", channelNumber);
  const uint16_t delay = 500;
  sendCommandUntilInData(command, delay, "OK");
}

static inline void sendHttpResponse(const char *contentType, uint8_t channelNumber, const char *content) {
  char contentChunk[SEND_DATA_MAX_LENGTH + 1] = {0};
  char headers[HEADERS_MAX_LENGTH] = {0};
  (void)sprintf(headers, "HTTP/1.1 200 OK\ncontent-type:%s\n\n", contentType);
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPSEND=%d,%d\r\n", channelNumber, strlen(headers));
  char data[GET_DATA_MAX_LENGTH] = {0};
  uint16_t contentLength = strlen(content);
  sendData(command);
  getData(data);
  if (!strstr(data, ">")) {
    closeChannel(channelNumber);
    return;
  }
  sendData(headers);
  for (size_t i = 0; i < contentChunk; i += SEND_DATA_MAX_LENGTH) {
    uint16_t currentChunkSize = contentLength >= SEND_DATA_MAX_LENGTH + i ? SEND_DATA_MAX_LENGTH : contentLength - i;
    strncpy(contentChunk, content + i, currentChunkSize);
    contentChunk[currentChunkSize] = '\0';
    char command[COMMAND_MAX_LENGTH] = {0};
    (void)sprintf(command, "AT+CIPSEND=%d,%d\r\n", channelNumber, strlen(contentChunk));
    char data[GET_DATA_MAX_LENGTH] = {0};
    sendData(command);
    getData(data);
    if (!strstr(data, ">")) {
      closeChannel(channelNumber);
      return;
    }
    sendData(contentChunk);
  }
  closeChannel(channelNumber);
}

int8_t runEsp8266() {
  char data[GET_DATA_MAX_LENGTH] = {0};
  getData(data);
  int8_t channelNumber = getChannelNumber(data);
  if (channelNumber == -1) {
    return -1;
  }
  if (strstr(data, "GET /api HTTP")) {
    return channelNumber;
  }
  if (strstr(data, "GET / HTTP")) {
    sendHttpResponse("text/html", channelNumber, html);
  } else {
    closeChannel(channelNumber);
  }
  return -1;
}

void handleApiRequest(uint8_t channelNumber, const DHT11 *dht11Ptr, uint32_t photoresistorValue) {
  char response[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(response, "{\"dht11\":{\"ok\":%d,\"t\":%d,\"h\":%d},\"l\":%ld}", !dht11Ptr->status,
                dht11Ptr->temperature, dht11Ptr->humidity, photoresistorValue);
  sendHttpResponse("application/json", channelNumber, response);
}
