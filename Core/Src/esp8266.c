#include "esp8266.h"

uint8_t rxBuffer[512] = {0};
uint8_t ATisOK;
int channel;
int onoff;
int led = 1;
char ATcommand[64];
char ATcommandB[1024];
char ATcommandN[100];
char ATcommandF[100];
char ATcommandT[16];
int countB;
int countN;
int countF;
int countT;

void initialiseEsp8266() {
  sprintf(ATcommandB, "<!DOCTYPE html><html>\n<head>\n\
  <title>STM32 - ESP8266</title>\n<link href=\"data:image/x-icon;base64,\
  A\" rel=\"icon\" type=\"image/x-icon\"><style>\nhtml {\
  display: inline-block; margin: 0px auto; text-align: center;}\n\
  body{margin-top: 50px;}\n.button {display: block;\n\
  width: 70px;\nbackground-color: #008000;\nborder: none;\ncolor: white;\n\
  padding: 14px 28px;\ntext-decoration: none;\nfont-size: 24px;\n\
  margin: 0px auto 36px; \nborder-radius: 5px;}\n\
  .button-on {background-color: #008000;}\n.button-on:active\
  {background-color: #008000;}\n.button-off {background-color: #808080;}\n\
  .button-off:active {background-color: #808080;}\n\
  p {font-size: 14px;color: #808080;margin-bottom: 20px;}\n\
  </style>\n</head>\n<body>\n<h1>STM32 - ESP8266</h1>");
  sprintf(ATcommandN, "<p>Light is currently on\
  </p><a class=\"button button-off\" href=\"/lightoff\">OFF</a>");
  sprintf(ATcommandF, "<p>Light is currently off\
  </p><a class=\"button button-on\" href=\"/lighton\">ON</a>");
  sprintf(ATcommandT, "</body></html>");
  countB = strlen(ATcommandB);
  countN = strlen(ATcommandN);
  countF = strlen(ATcommandF);
  countT = strlen(ATcommandT);

  sprintf(ATcommand, "AT+RST\r\n");
  memset(rxBuffer, 0, sizeof(rxBuffer));
  HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
  HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
  HAL_Delay(500);

  ATisOK = 0;
  while (!ATisOK) {
    sprintf(ATcommand, "AT+CWMODE_CUR=2\r\n");
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
    HAL_UART_Receive(&huart1, rxBuffer, 512, 1000);
    if (strstr((char *)rxBuffer, "OK")) {
      ATisOK = 1;
    }
    HAL_Delay(500);
  }

  ATisOK = 0;
  while (!ATisOK) {
    sprintf(ATcommand, "AT+CWSAP_CUR=\"STM32\",\"12345678\",1,3,4,0\r\n");
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
    HAL_UART_Receive(&huart1, rxBuffer, 512, 1000);
    if (strstr((char *)rxBuffer, "OK")) {
      ATisOK = 1;
    }
    HAL_Delay(500);
  }

  ATisOK = 0;
  while (!ATisOK) {
    sprintf(ATcommand, "AT+CIPAP_CUR=\"192.168.51.1\"\r\n");
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
    HAL_UART_Receive(&huart1, rxBuffer, 512, 1000);
    if (strstr((char *)rxBuffer, "OK")) {
      ATisOK = 1;
    }
    HAL_Delay(500);
  }

  ATisOK = 0;
  while (!ATisOK) {
    sprintf(ATcommand, "AT+CIPMUX=1\r\n");
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
    HAL_UART_Receive(&huart1, rxBuffer, 512, 1000);
    if (strstr((char *)rxBuffer, "OK")) {
      ATisOK = 1;
    }
    HAL_Delay(500);
  }

  ATisOK = 0;
  while (!ATisOK) {
    sprintf(ATcommand, "AT+CIPSERVER=1,80\r\n");
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
    HAL_UART_Receive(&huart1, rxBuffer, 512, 1000);
    if (strstr((char *)rxBuffer, "OK")) {
      ATisOK = 1;
    }
    HAL_Delay(500);
  }
}

void runEsp8266() {
  while (1) {
    memset(rxBuffer, 0, sizeof(rxBuffer));
    HAL_UART_Receive(&huart1, rxBuffer, 512, 1000);
    if (strstr((char *)rxBuffer, "+IPD,0"))
      channel = 0;
    else if (strstr((char *)rxBuffer, "+IPD,1"))
      channel = 1;
    else if (strstr((char *)rxBuffer, "+IPD,2"))
      channel = 2;
    else if (strstr((char *)rxBuffer, "+IPD,3"))
      channel = 3;
    else if (strstr((char *)rxBuffer, "+IPD,4"))
      channel = 4;
    else if (strstr((char *)rxBuffer, "+IPD,5"))
      channel = 5;
    else if (strstr((char *)rxBuffer, "+IPD,6"))
      channel = 6;
    else if (strstr((char *)rxBuffer, "+IPD,7"))
      channel = 7;
    else
      channel = 100;

    if (strstr((char *)rxBuffer, "GET /lighton"))
      onoff = 0;
    else if (strstr((char *)rxBuffer, "GET /lightoff"))
      onoff = 1;
    else
      onoff = led;

    if (channel < 8 && onoff == 1) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
      led = 1;
      sprintf(ATcommand, "AT+CIPSEND=%d,%d\r\n", channel, countB + countF + countT);
      memset(rxBuffer, 0, sizeof(rxBuffer));
      HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
      HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
      if (strstr((char *)rxBuffer, ">")) {
        memset(rxBuffer, 0, sizeof(rxBuffer));
        HAL_UART_Transmit(&huart1, (uint8_t *)ATcommandB, countB, 1000);
        HAL_UART_Transmit(&huart1, (uint8_t *)ATcommandF, countF, 1000);
        HAL_UART_Transmit(&huart1, (uint8_t *)ATcommandT, countT, 1000);
        HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
      }
      sprintf(ATcommand, "AT+CIPCLOSE=%d\r\n", channel);
      memset(rxBuffer, 0, sizeof(rxBuffer));
      HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
      HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
      channel = 100;
    } else if (channel < 8 && onoff == 0) {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
      led = 0;
      sprintf(ATcommand, "AT+CIPSEND=%d,%d\r\n", channel, countB + countN + countT);
      memset(rxBuffer, 0, sizeof(rxBuffer));
      HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
      HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
      if (strstr((char *)rxBuffer, ">")) {
        memset(rxBuffer, 0, sizeof(rxBuffer));
        HAL_UART_Transmit(&huart1, (uint8_t *)ATcommandB, countB, 1000);
        HAL_UART_Transmit(&huart1, (uint8_t *)ATcommandN, countN, 1000);
        HAL_UART_Transmit(&huart1, (uint8_t *)ATcommandT, countT, 1000);
        HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
      }
      sprintf(ATcommand, "AT+CIPCLOSE=%d\r\n", channel);
      memset(rxBuffer, 0, sizeof(rxBuffer));
      HAL_UART_Transmit(&huart1, (uint8_t *)ATcommand, strlen(ATcommand), 1000);
      HAL_UART_Receive(&huart1, rxBuffer, 512, 100);
      channel = 100;
    }
  }
}
