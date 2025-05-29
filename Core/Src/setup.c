#include "setup.h"
#include "esp8266.h"
#include "terminal.h"
#include <stdio.h>

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  initialiseEsp8266();
}