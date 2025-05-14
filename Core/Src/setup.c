#include "setup.h"

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  runPhotoresitorAsync();
  initialiseEsp8266();
}