#include "esp8266.h"
#include "html.h"
#include "utility.h"
#include <stdio.h>
#include <string.h>

enum { DATA_MAX_LENGTH = 512, COMMAND_MAX_LENGTH = 64 };

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
  for (uint32_t i = 0; i < 4; ++i) {
    (void)sprintf(substring, "+IPD,%lu", i);
    if (strstr(data, substring)) {
      return i;
    }
  }
  return -1;
}

static inline void closeConnection(uint32_t linkId) {
  const uint8_t delay = 150;
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPCLOSE=%lu\r\n", linkId);
  HAL_Delay(delay);
  send(command);
}

Esp8266 runEsp8266() {
  char data[DATA_MAX_LENGTH] = {0};
  receive(data);
  uint32_t linkId = getLinkId(data);
  if (linkId == -1) {
    return (Esp8266){-1};
  }
  if (strstr(data, "GET /api HTTP")) {
    return (Esp8266){linkId, "application/json"};
  }
  if (strstr(data, "GET / HTTP")) {
    return (Esp8266){linkId, "text/html"};
  }
  closeConnection(linkId);
  return (Esp8266){-1};
}

static inline void sendHttpResponse(const Esp8266 *esp8266Ptr, const char *body) {
  enum { RESPONSE_MAX_LENGTH = 2048 };
  char response[RESPONSE_MAX_LENGTH] = {0};
  (void)sprintf(response, "HTTP/1.1 200 OK\ncontent-type:%s\n\n%s", esp8266Ptr->contentType, body);
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPSEND=%lu,%d\r\n", esp8266Ptr->linkId, strlen(response));
  const uint8_t delay = 20;
  send(command);
  HAL_Delay(delay);
  send(response);
  closeConnection(esp8266Ptr->linkId);
}

void handleRequest(const Esp8266 *esp8266Ptr, const DHT11 *dht11Ptr, uint32_t photoresistorValue) {
  enum { CONTENT_MAX_LENGTH = 2048 };
  char body[CONTENT_MAX_LENGTH] = {0};
  const char *template = NULL;
  if (!strcmp(esp8266Ptr->contentType, "application/json")) {
    template = "{\"th\":{\"ok\":%d,\"t\":%d,\"h\":%d},\"l\":%ld}";
  } else if (!strcmp(esp8266Ptr->contentType, "text/html")) {
    template = htmlTemplate;
  } else {
    closeConnection(esp8266Ptr->linkId);
    return;
  }
  (void)sprintf(body, template, !dht11Ptr->status, dht11Ptr->temperature, dht11Ptr->humidity, photoresistorValue);
  sendHttpResponse(esp8266Ptr, body);
}
