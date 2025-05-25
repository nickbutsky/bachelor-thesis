#include "esp8266.h"

enum { DATA_MAX_LENGTH = 512, COMMAND_MAX_LENGTH = 64 };

uint8_t linkIdToConnected[] = {0, 0, 0, 0};

static inline void receive(char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Receive(&huart1, (uint8_t *)data, DATA_MAX_LENGTH, timeout);
}

static inline void send(const char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Transmit(&huart1, (uint8_t *)data, strlen(data), timeout);
}

void initialiseEsp8266() {
  const char *commands[] = {"AT+RST\r\n",
                            "AT+CWMODE_CUR=2\r\n",
                            "AT+CWSAP_CUR=\"MICROCLIMATE_CONTROL\",\"12345678\",1,3,4\r\n",
                            "AT+CIPAP_CUR=\"192.168.51.1\"\r\n",
                            "AT+CIPMUX=1\r\n",
                            "AT+CIPSERVER=1,80\r\n"};
  const uint16_t delay = 500;
  for (uint32_t i = 0; i < lengthof(commands); ++i) {
    send(commands[i]);
    HAL_Delay(delay);
  }
}

static inline uint32_t getLinkId(const char *data) {
  enum { SUBSTRING_LENGTH = 7 };
  char substring[SUBSTRING_LENGTH] = {0};
  for (uint32_t i = 0; i < lengthof(linkIdToConnected); ++i) {
    (void)sprintf(substring, "+IPD,%lu", i);
    if (strstr(data, substring)) {
      linkIdToConnected[i] = 1;
      return i;
    }
  }
  return -1;
}

static inline void closeConnections() {
  for (uint32_t linkId = 0; linkId < lengthof(linkIdToConnected); ++linkId) {
    if (linkIdToConnected[linkId]) {
      continue;
    }
    char command[COMMAND_MAX_LENGTH] = {0};
    (void)sprintf(command, "AT+CIPCLOSE=%lu\r\n", linkId);
    const uint8_t upperBound = 8;
    const uint8_t delay = 20;
    for (uint8_t i = 0; i < upperBound; ++i) {
      send(command);
      HAL_Delay(delay);
    }
  }
}

Esp8266 runEsp8266() {
  char data[DATA_MAX_LENGTH] = {0};
  receive(data);
  uint32_t linkId = getLinkId(data);
  if (linkId == -1) {
    closeConnections();
    return (Esp8266){-1};
  }
  if (strstr(data, "GET /api HTTP")) {
    return (Esp8266){linkId, "application/json"};
  }
  if (strstr(data, "GET / HTTP")) {
    return (Esp8266){linkId, "text/html"};
  }
  closeConnections();
  return (Esp8266){-1};
}

static inline void sendHttpResponse(const Esp8266 *esp8266Ptr, const char *content) {
  enum { RESPONSE_MAX_LENGTH = 2048 };
  char response[RESPONSE_MAX_LENGTH] = {0};
  (void)sprintf(response, "HTTP/1.1 200 OK\ncontent-type:%s\n\n%s", esp8266Ptr->contentType, content);
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPSEND=%lu,%d\r\n", esp8266Ptr->linkId, strlen(response));
  const uint8_t smallerDelay = 20;
  const uint16_t biggerDelay = 500;
  send(command);
  HAL_Delay(smallerDelay);
  send(response);
  HAL_Delay(biggerDelay);
  linkIdToConnected[esp8266Ptr->linkId] = 0;
}

void handleRequest(const Esp8266 *esp8266Ptr, const DHT11 *dht11Ptr, uint32_t photoresistorValue) {
  enum { CONTENT_MAX_LENGTH = 2048 };
  char content[CONTENT_MAX_LENGTH] = {0};
  if (!strcmp(esp8266Ptr->contentType, "application/json")) {
    (void)sprintf(content, "{\"th\":{\"ok\":%d,\"t\":%d,\"h\":%d},\"l\":%ld}", !dht11Ptr->status, dht11Ptr->temperature,
                  dht11Ptr->humidity, photoresistorValue);
  } else if (!strcmp(esp8266Ptr->contentType, "text/html")) {
    (void)sprintf(content, htmlTemplate, dht11Ptr->temperature, dht11Ptr->humidity, photoresistorValue);
  }
  sendHttpResponse(esp8266Ptr, content);
}
