#include <ESP8266WiFi.h>
#include "config.h"

void start_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to wifi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }

  Serial.println();
  Serial.println(WiFi.localIP());
}
