#define DEBUG
#include "public.h"
#include "log.h"
wLog logs;

void setup() {
  // put your setup code here, to run once:
  comm_init();
}

void loop() {
  get_data();
  stepSML();
}
