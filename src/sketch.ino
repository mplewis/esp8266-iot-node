#include "wifi.h"
#include "mqtt.h"

long led_module_off_at = 0;
long led_board_off_at = 0;

void setup() {
  pinMode(PIN_LED_BOARD, OUTPUT);
  digitalWrite(PIN_LED_BOARD, HIGH);

  Serial.begin(115200);
  Serial.println();

  start_wifi();
  start_mqtt();

  mqtt.setCallback(mqtt_in);
}

void loop() {
  if (millis() % 5000 == 0) {
    blink(100);
    publish("Hello world!");
  }

  handle_leds();
  mqtt.loop();
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

void mqtt_in(char *topic, uint8_t *payload, unsigned int len) {
  Serial.println("Topic:");
  Serial.println(topic);
  Serial.println("Payload:");
  for (unsigned int i = 0; i < len; i++) {
    Serial.write(payload[i]);
  }
  Serial.println();
  blink(100);
}
