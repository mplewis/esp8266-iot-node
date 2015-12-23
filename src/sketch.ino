#include "wifi.h"
#include "cmds.h"
#include "led_matrix.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  start_leds();
  start_led_matrix();
  start_wifi();
  scroll_forever("Hello world!");
}

void loop() {
  handle_leds();
  handle_led_matrix();
  handle_wifi();
}
