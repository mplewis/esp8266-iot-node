#include "wifi.h"
#include "cmds.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  start_leds();
  start_wifi();
}

void loop() {
  handle_leds();
  handle_wifi();
}
