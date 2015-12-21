#include <ESP8266WiFi.h>
#include "config.h"
#include "leds.h"

#define BLINK_PIN_WIFI PIN_LED_MODULE

void start_wifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  start_leds();
  static bool blink_state = LOW;

  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    for (char i = 0; i < WIFI_NETWORK_COUNT; i++) {
      const char *ssid = WIFI_SSIDS[i];
      const char *password = WIFI_PASSWORDS[i];
      WiFi.begin(ssid, password);

      Serial.print("Connecting to ");
      Serial.print(ssid);
      Serial.print("...");

      unsigned long try_next_at = millis() + WIFI_CONNECT_TIMEOUT;
      while (WiFi.status() != WL_CONNECTED && millis() < try_next_at) {
        delay(200);
        Serial.print(".");
        digitalWrite(BLINK_PIN_WIFI, blink_state);
        blink_state = !blink_state;
      }
      Serial.println();
    }
  }

  digitalWrite(BLINK_PIN_WIFI, LOW);

  Serial.println();
  Serial.println("Wifi ready.");
  Serial.println(WiFi.localIP());
}

void maintain_wifi() {
  start_wifi();
}
