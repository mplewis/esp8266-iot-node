#include "leds.h"
#include "wifi.h"
#include "led_matrix.h"
#include "http.h"

void setup() {
  Serial.begin(115200);
  Serial.println();  // Clear the garbage from ESP8266's ROM

  start_leds();
  start_led_matrix();
  start_wifi();
  http_set_request_cb(router);
}

void loop() {
  handle_leds();
  handle_led_matrix();
  handle_wifi();
}

void router(WiFiClient client, const char *method, const char *path, const char *body) {
  if (strcmp(path, "/") == 0) {
    if (strcmp(method, "GET") == 0) {
      http_respond(client, RC_200, "OK");
    } else {
      http_respond(client, RC_405);
    }

  } else if (strcmp(path, "/matrix") == 0) {
    if (strcmp(method, "POST") == 0) {
      http_respond(client, RC_204);
      scroll_once(body);
    } else {
      http_respond(client, RC_405);
    }

  } else {
    http_respond(client, RC_404);
  }
}
