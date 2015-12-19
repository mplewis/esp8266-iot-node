#include <ESP8266WiFi.h>
#include "config.h"

#define BLINK_PIN PIN_LED_MODULE

void start_wifi() {
  static bool blink = LOW;
  pinMode(BLINK_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  Serial.print("...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
    digitalWrite(BLINK_PIN, blink);
    blink = !blink;
  }

  digitalWrite(BLINK_PIN, LOW);

  Serial.println();
  Serial.println("Wifi ready.");
  Serial.println(WiFi.localIP());
}
