#include "esp8266.h"

enum { DATA_MAX_LENGTH = 512, COMMAND_MAX_LENGTH = 64 };

static inline void receive(char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Receive(&huart1, (uint8_t *)data, DATA_MAX_LENGTH, timeout);
  puts("received");
  puts(data);
}

static inline void send(const char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Transmit(&huart1, (uint8_t *)data, strlen(data), timeout);
  puts("sent");
  puts(data);
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
    send(commands[i]);
    HAL_Delay(delay);
  }
}

static inline void closeChannel(uint8_t channelNumber) {
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPCLOSE=%d\r\n", channelNumber);
  for (uint8_t i = 0; i < 16; ++i) {
    send(command);
    HAL_Delay(20);
  }
}

static inline void sendHttpResponse(const char *contentType, uint8_t channelNumber, const char *content) {
  enum { RESPONSE_MAX_LENGTH = 2048 };
  char response[RESPONSE_MAX_LENGTH] = {0};
  (void)sprintf(response, "HTTP/1.1 200 OK\ncontent-type:%s\n\n%s", contentType, content);
  char command[COMMAND_MAX_LENGTH] = {0};
  (void)sprintf(command, "AT+CIPSEND=%d,%d\r\n", channelNumber, strlen(response));
  send(command);
  send(response);
  closeChannel(channelNumber);
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

int8_t runEsp8266() {
  char data[DATA_MAX_LENGTH] = {0};
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
  (void)sprintf(response, "{\"th\":{\"ok\":%d,\"t\":%d,\"h\":%d},\"l\":%ld}", !dht11Ptr->status, dht11Ptr->temperature,
                dht11Ptr->humidity, photoresistorValue);
  sendHttpResponse("application/json", channelNumber, response);
}
