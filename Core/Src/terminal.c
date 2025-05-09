#include "terminal.h"

char buffer[BUFFER_LENGTH];
uint8_t getIndex;
uint8_t putIndex;
volatile char rxCharacter;

void initialiseTerminal() {
  putIndex = getIndex = 0;
  buffer[0] = 0;
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCharacter, 1);
}

void printEverything() {
  while (putIndex - getIndex) {
    char character = '\0';
    if (putIndex != getIndex) {
      character = buffer[getIndex];
      getIndex = BUFFER_LENGTH > getIndex + 1 ? getIndex + 1 : 0;
    }
    __io_putchar(character);
  }
}

int __io_putchar(int ch) {
  const uint8_t offset = 7;
  while (1) {
    if (USART2->SR & (0x1UL << offset)) {
      break;
    }
  }
  USART2->DR = (uint8_t)ch;
  return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart != &huart2) {
    return;
  }
  buffer[putIndex] = rxCharacter;
  putIndex = BUFFER_LENGTH > putIndex + 1 ? putIndex + 1 : 0;
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCharacter, 1);
}
