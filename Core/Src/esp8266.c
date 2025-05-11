#include "esp8266.h"

enum { DATA_MAX_LENGTH = 512, SUBSTRING_LENGTH = 7, COMMAND_LENGTH = 64 };

const char *const htmlParts[] = {
    "<!DOCTYPE html><html><head> <title>STM32 - ESP8266</title>\n<link href=\"data:image/x-icon;base64,A\" ",
    "rel=\"icon\" type=\"image/x-icon\"><style>\nhtml {display: inline-block; margin: 0px auto; text-align: ",
    "center;}body{margin-top: 50px;}\n.button {display: block;width: 70px;\nbackground-color: #008000;\nborder: ",
    "none;\ncolor: white;padding: 14px 28px;\ntext-decoration: none;\nfont-size: 24px; margin: 0px auto 36px; ",
    "\nborder-radius: 5px;} .button-on {background-color: #008000;}\n.button-on:active{background-color: ",
    "#008000;}\n.button-off {background-color: #808080;}.button-off:active {background-color: #808080;}p {font-size: ",
    "14px;color: #808080;margin-bottom: 20px;}</style>\n</head>\n<body>\n<h1>STM32 - ESP8266</h1></body></html>"};

static inline void getData(char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Receive(&huart1, (uint8_t *)data, DATA_MAX_LENGTH, timeout);
}

static inline void sendData(const char *data) {
  const uint16_t timeout = 1000;
  HAL_UART_Transmit(&huart1, (uint8_t *)data, strlen(data), timeout);
}

void initialiseEsp8266() {
  sendData("AT+RST\r\n");
  const uint16_t delay = 500;
  HAL_Delay(delay);
  const char *commands[] = {"AT+CWMODE_CUR=2\r\n", "AT+CWSAP_CUR=\"STM32\",\"12345678\",1,3,4,0\r\n",
                            "AT+CIPAP_CUR=\"192.168.51.1\"\r\n", "AT+CIPMUX=1\r\n", "AT+CIPSERVER=1,80\r\n"};
  for (uint32_t i = 0; i < lengthof(commands); ++i) {
    while (1) {
      char response[DATA_MAX_LENGTH] = {0};
      sendData(commands[i]);
      getData(response);
      if (strstr(response, "OK")) {
        break;
      }
      HAL_Delay(delay);
    }
  }
}

static inline int8_t getChannelNumber(const char *data) {
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

void runEsp8266() {
  char data[DATA_MAX_LENGTH] = {0};
  getData(data);
  int8_t channelNumber = getChannelNumber(data);
  if (channelNumber == -1) {
    return;
  }
  char command[COMMAND_LENGTH] = {0};
  for (uint32_t i = 0; i < lengthof(htmlParts); ++i) {
    (void)sprintf(command, "AT+CIPSEND=%d,%d\r\n", channelNumber, strlen(htmlParts[i]));
    sendData(command);
    HAL_Delay(1);
    sendData(htmlParts[i]);
    HAL_Delay(100);
  }
  (void)sprintf(command, "AT+CIPCLOSE=%d\r\n", channelNumber);
  sendData(command);
}
