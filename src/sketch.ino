#include "config.h"

#include <espretty_leds.h>
#include <espretty_wifi.h>
#include <espretty_http.h>

void setup() {
  Serial.begin(115200);
  Serial.println();  // Clear the garbage from ESP8266's ROM

  start_leds();
  start_wifi();
  http_set_request_cb(router);

  digitalWrite(PIN_LED_BOARD, HIGH);
}

void loop() {
  handle_wifi();
}

void router(WiFiClient client, const char *method, const char *path, const char *body) {
  if (strcmp(path, "/") == 0) {
    if (strcmp(method, "GET") == 0) {
      http_respond(client, RC_200, "OK");
    } else {
      http_respond(client, RC_405);
    }

  } else if (strcmp(path, "/light") == 0) {
    if (strcmp(method, "POST") == 0) {
      if (strcmp(body, "0") == 0) {
        digitalWrite(PIN_LED_BOARD, HIGH);
        http_respond(client, RC_204);
      } else if (strcmp(body, "1") == 0) {
        digitalWrite(PIN_LED_BOARD, LOW);
        http_respond(client, RC_204);
      } else {
        Serial.println("Unknown POST to /light:");
        Serial.write(body);
        http_respond(client, RC_500);
      }
    } else if (strcmp(method, "GET") == 0) {
      if (digitalRead(PIN_LED_BOARD)) {
        http_respond(client, RC_200, "0");
      } else {
        http_respond(client, RC_200, "1");
      }
    } else {
      http_respond(client, RC_405);
    }

  } else {
    http_respond(client, RC_404);
  }
}
