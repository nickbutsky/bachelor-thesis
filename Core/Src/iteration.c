#include "iteration.h"

void iteration() {
  // const char *message = "Hello world";
  while (RX_available()) {
    __io_putchar(RX_read());
  }
  // const uint8_t delay = 1000;
  // HAL_Delay(delay);
  // // HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
  // printf("based");
  HAL_Delay(1);
}