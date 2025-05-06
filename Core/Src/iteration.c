#include "iteration.h"

void iteration() {
  // const char *message = "Hello world";
  const uint8_t delay = 1000;
  HAL_Delay(delay);
  HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
}