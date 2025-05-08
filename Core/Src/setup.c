#include "setup.h"

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  LIGHT_stat = _LIGHT_START;
}