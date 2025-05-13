#include "setup.h"

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  updatePhotoresistorValue();
  initialiseEsp8266();
}