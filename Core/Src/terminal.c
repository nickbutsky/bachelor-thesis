#include "terminal.h"

char rxBuffer[RX_BUFFER_SIZE];
uint8_t rxPutIndex;
uint8_t rxGetIndex;
volatile char rxCharacter;

int __io_putchar(int ch) {
  const uint8_t offset = 7;
  while (1) {
    if (USART2->SR & (0x1UL << offset)) {
      break;
    }
  }
  USART2->DR = (uint8_t)ch;
  // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart != &huart2) {
    return;
  }
  rxBuffer[rxPutIndex] = rxCharacter;
  rxPutIndex = RX_BUFFER_SIZE > rxPutIndex + 1 ? rxPutIndex + 1 : 0;
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCharacter, 1);
}

void initialiseRx() {
  rxPutIndex = rxGetIndex = 0;
  rxBuffer[0] = 0;
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCharacter, 1);
}

void exhaustRx() {
  while (rxPutIndex - rxGetIndex) {
    char character = '\0';
    if (rxPutIndex != rxGetIndex) {
      character = rxBuffer[rxGetIndex];
      rxGetIndex = RX_BUFFER_SIZE > rxGetIndex + 1 ? rxGetIndex + 1 : 0;
    }
    __io_putchar(character);
  }
}
