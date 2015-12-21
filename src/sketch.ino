#include "wifi.h"
#include "mqtt.h"
#include "cmds.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  start_leds();
  start_wifi();
  start_mqtt();
  start_cmds();
}

void loop() {
  handle_leds();
  maintain_wifi();
  maintain_mqtt();
}
