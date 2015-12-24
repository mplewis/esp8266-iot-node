#include "leds.h"
#include "wifi.h"
#include "led_matrix.h"

void setup() {
  Serial.begin(115200);
  Serial.println();  // Clear the garbage from ESP8266's ROM

  start_leds();
  start_led_matrix();
  start_wifi();
}

void loop() {
  handle_leds();
  handle_led_matrix();
  handle_wifi();
}
