#include "wifi.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  start_leds();
  start_wifi();
  start_mqtt();

  mqtt.setCallback(mqtt_in);
}

void loop() {
  if (millis() % 5000 == 0) {
    publish("Hello world!");
  }

  handle_leds();
  maintain_wifi();
  mqtt.loop();
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
