#include <ESP8266WiFi.h>
#include "config.h"
#include "leds.h"

#define BLINK_PIN_WIFI PIN_LED_MODULE

void start_wifi() {
  start_leds();
  static bool blink = LOW;

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  Serial.print("...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    digitalWrite(BLINK_PIN_WIFI, blink);
    blink = !blink;
  }

  digitalWrite(BLINK_PIN_WIFI, LOW);

  Serial.println();
  Serial.println("Wifi ready.");
  Serial.println(WiFi.localIP());
}
