#include "setup.h"

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  photoresistorStatus = PHOTORESISTOR_STARTING;
}