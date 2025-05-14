#include "setup.h"

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  runPhotoresistorAsync();
  initialiseEsp8266();
}