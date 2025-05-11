#include "terminal.h"

enum { BUFFER_LENGTH = 32 };

char buffer[BUFFER_LENGTH];
uint8_t getIndex;
uint8_t putIndex;
char rxCharacter;

void initialiseTerminal() { HAL_UART_Receive_IT(&huart2, (uint8_t *)&rxCharacter, 1); }

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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart != &huart2) {
    return;
  }
  buffer[putIndex] = rxCharacter;
  putIndex = BUFFER_LENGTH > putIndex + 1 ? putIndex + 1 : 0;
  initialiseTerminal();
}
