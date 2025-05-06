#include "iteration.h"

void iteration() {
  while (isRxAvailable()) {
    __io_putchar(readRx());
  }
  HAL_Delay(1);
}