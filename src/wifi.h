#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "config.h"
#include "leds.h"
#include "led_matrix.h"

#define BLINK_PIN_WIFI PIN_LED_MODULE
const long UDP_BROADCAST_IP = 0xFFFFFFFF;  // 255.255.255.255
const int BUF_SIZE = 1024;

void parse_request(char *buf);  // cmds.h
void advertise();
void listen_for_cmds();

WiFiUDP udp;
WiFiServer rest(80);
unsigned long advertise_at = 0;
char buf[BUF_SIZE];

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

  rest.begin();
  digitalWrite(BLINK_PIN_WIFI, LOW);

  Serial.println();
  Serial.println("Wifi ready.");
  Serial.println(WiFi.localIP());
  scroll_once(WiFi.localIP().toString());
}

void handle_wifi() {
  start_wifi();
  advertise();
  listen_for_cmds();
}

void advertise() {
  if (millis() < advertise_at) {
    return;
  }
  advertise_at = millis() + WIFI_ADVERTISING_INTERVAL;
  blink(100);
  udp.beginPacket(UDP_BROADCAST_IP, WIFI_ADVERTISING_PORT);
  String reply = "";
  reply += WiFi.localIP().toString();
  reply += ",";
  reply += BOARD_NAME;
  reply += ",";
  reply += BOARD_DESC;
  reply.toCharArray(buf, BUF_SIZE);
  udp.write(buf);
  udp.endPacket();
}

void listen_for_cmds() {
  WiFiClient client = rest.available();
  if (!client) return;

  blink(100);

  // Wait for the request body to come in
  unsigned long wait_until = millis() + WIFI_CLIENT_DATA_TIMEOUT;
  while (!client.available() && millis() < wait_until) {
    delay(1);
  }

  // Sometimes clients send headers then send the body.
  // Give the client a little time to get all its data out the wire
  // Empirically, most clients get data out within 5 ms
  // I've seen clients take up to 12 ms to finish sending data
  // This is a lazy hack - we should be checking Content-Length instead
  if (WIFI_DEBUG_HTTP_TIMING) {
    int last_avail = client.available();
    // Print how long it takes us to get additional bytes after first byte
    for (char i = 0; i < 100; i++) {
      if (client.available() > last_avail) {
        int received = client.available() - last_avail;
        last_avail = client.available();
        Serial.print(i, DEC);
        Serial.print(": ");
        Serial.println(received);
      }
      delay(1);
    }
  } else {
    delay(40);
  }

  // Read request into buffer
  int pos = 0;
  while (client.available()) {
    char b = client.read();
    buf[pos++] = b;
    if (pos == BUF_SIZE - 1) break;
  }
  buf[pos] = 0;  // terminate string

  // Write out the response
  client.write("HTTP/1.1 204 No Content\r\n\r\n");
  client.flush();

  // Parse the request and print to serial
  parse_request(buf);
}
