#include "esp8266.h"

enum { OUTPUT_MAX_LENGTH = 512, COMMAND_MAX_LENGTH = 64 };

static inline void receive(char *output) {
  const uint16_t timeout = 1000;
  HAL_UART_Receive(&huart1, (uint8_t *)output, OUTPUT_MAX_LENGTH, timeout);
}

static inline void send(const char *input, char *output, uint16_t delay) {
  const uint16_t timeout = 1000;
  HAL_UART_Transmit(&huart1, (uint8_t *)input, strlen(input), timeout);
  HAL_Delay(delay);
  receive(output);
}

static inline void sendUntilInOutput(const char *input, uint16_t delay, const char *substring) {
  while (1) {
    char output[OUTPUT_MAX_LENGTH] = {0};
    send(input, output, 0);
    if (strstr(output, substring)) {
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
    sendUntilInOutput(commands[i], delay, "OK");
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
  sendUntilInOutput(command, delay, "OK");
}

static inline uint8_t cipsend(uint8_t channelNumber, const char *data) {
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPSEND=%d,%d\r\n", channelNumber, strlen(data));
  char commandResponse[OUTPUT_MAX_LENGTH] = {0};
  send(command, commandResponse, 1);
  if (!strstr(commandResponse, ">")) {
    closeChannel(channelNumber);
    return 1;
  }
  send(data, NULL, 100);
  return 0;
}

static inline void sendHttpResponse(const char *contentType, uint8_t channelNumber, const char *content) {
  enum { HEADERS_MAX_LENGTH = 128 };
  char headers[HEADERS_MAX_LENGTH] = {0};
  (void)sprintf(headers, "HTTP/1.1 200 OK\ncontent-type:%s\n\n", contentType);
  enum { SEND_DATA_MAX_LENGTH = 2048 };
  if (strlen(headers) + strlen(content) <= SEND_DATA_MAX_LENGTH) {
    char response[SEND_DATA_MAX_LENGTH] = {0};
    (void)sprintf(response, "%s%s", headers, content);
    cipsend(channelNumber, response);
    closeChannel(channelNumber);
    return;
  }
  uint32_t contentLength = strlen(content);
  char chunk[SEND_DATA_MAX_LENGTH + 1] = {0};
  if (cipsend(channelNumber, headers)) {
    return;
  }
  for (uint32_t i = 0; i < contentLength; i += SEND_DATA_MAX_LENGTH) {
    uint16_t currentChunkSize = contentLength >= SEND_DATA_MAX_LENGTH + i ? SEND_DATA_MAX_LENGTH : contentLength - i;
    strncpy(chunk, content + i, currentChunkSize);
    chunk[currentChunkSize] = '\0';
    if (cipsend(channelNumber, chunk)) {
      return;
    }
  }
  closeChannel(channelNumber);
}

int8_t runEsp8266() {
  char data[OUTPUT_MAX_LENGTH] = {0};
  receive(data);
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
  enum { RESPONSE_MAX_LENGTH = 128 };
  char response[RESPONSE_MAX_LENGTH] = {0};
  (void)sprintf(response, "{\"dht11\":{\"ok\":%d,\"t\":%d,\"h\":%d},\"l\":%ld}", !dht11Ptr->status,
                dht11Ptr->temperature, dht11Ptr->humidity, photoresistorValue);
  sendHttpResponse("application/json", channelNumber, response);
}
