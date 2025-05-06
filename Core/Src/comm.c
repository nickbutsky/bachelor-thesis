#include "comm.h"

char RX_buff[_RX_BUF_SIZE];
uint8_t RX_put_ptr;
uint8_t RX_get_ptr;
volatile uint8_t RXchar;

// ============================================== PUTCHAR
int __io_putchar(int ch) {
  while ((USART2->SR & (0x1UL << 7)) == 0)
    ;
  USART2->DR = (uint8_t)ch;
  // bantik
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  return ch;
}

// ============================================== IRQ HANDLER
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if (huart == &huart2) {
    RX_buff[RX_put_ptr++] = RXchar;
    if (RX_put_ptr >= _RX_BUF_SIZE)
      RX_put_ptr = 0;
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&RXchar, 1);
  }
}

// ============================================== READIN GBUFFER
void RX_init(void) {
  RX_buff[RX_put_ptr = RX_get_ptr = 0] = 0;
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&RXchar, 1);
}

uint8_t RX_read(void) {
  if (RX_put_ptr != RX_get_ptr) {
    uint8_t ch = RX_buff[RX_get_ptr++];
    if (RX_get_ptr >= _RX_BUF_SIZE) {
      RX_get_ptr = 0;
    }
    return ch;
  }
  return 0;
}
