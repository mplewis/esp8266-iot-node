#ifndef __espretty_leds_h
#define __espretty_leds_h

#define PIN_LED_MODULE D4  // The active low LED on the ESP8266 module, by the metal can
#define PIN_LED_BOARD D0  // The active low LED on the NodeMCU board, by the reset button

long led_board_off_at = 0;
long led_module_off_at = 0;
bool led_first_init = true;

void start_leds() {
  pinMode(PIN_LED_BOARD, OUTPUT);
  pinMode(PIN_LED_MODULE, OUTPUT);
  if (led_first_init) {
    digitalWrite(PIN_LED_BOARD, HIGH);
    digitalWrite(PIN_LED_MODULE, HIGH);
    led_first_init = false;
  }
}

void blink(long duration) {
  digitalWrite(PIN_LED_BOARD, LOW);
  led_board_off_at = millis() + duration;
}

void blink_module(long duration) {
  digitalWrite(PIN_LED_MODULE, LOW);
  led_module_off_at = millis() + duration;
} 

void handle_leds() {
  if (led_board_off_at != 0 && millis() >= led_board_off_at) {
    led_board_off_at = 0;
    digitalWrite(PIN_LED_BOARD, HIGH);
  }
  if (led_module_off_at != 0 && millis() >= led_module_off_at) {
    led_module_off_at = 0;
    digitalWrite(PIN_LED_MODULE, HIGH);
  }
}

#endif
