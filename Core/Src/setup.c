#include "setup.h"

void setup() {
  initialiseTerminal();
  puts("APPLICATION START");
  photoresistor.status = PHOTORESISTOR_STARTING;
}