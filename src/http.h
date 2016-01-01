#ifndef __http_h
#define __http_h

#include <math.h>
#include "led_matrix.h"

const int REQUEST_LINE_SIZE = 256;
const int METHOD_SIZE = 16;
const int BODY_SIZE = 1024;

typedef enum ResponseCode {
  RC_200,
  RC_204,
  RC_404,
  RC_405,
  RC_500
} ResponseCode;

#define HTTP_1_1 "HTTP/1.1 "
#define RC_TEXT_200 "200 OK"
#define RC_TEXT_204 "204 No Content"
#define RC_TEXT_404 "404 Not Found"
#define RC_TEXT_405 "405 Method Not Allowed"
#define RC_TEXT_500 "500 Internal Server Error"
#define END_HEAD "\r\n\r\n"

void (*request_cb)(WiFiClient, const char *, const char *, const char *);

void handle_request(WiFiClient client, const char *method, const char *path, const char *body);
void respond(WiFiClient client, ResponseCode code);
void respond(WiFiClient client, ResponseCode code, const char *body);
void debug_request(const char *method, const char *path, const char *body);

void parse_request(WiFiClient client, char *buf) {
  char method[METHOD_SIZE];
  char path[REQUEST_LINE_SIZE];
  char body[BODY_SIZE];

  char *method_divider = strstr(buf, " ");
  // Add 1 byte for the terminator
  int method_len = method_divider - buf + 1;
  strlcpy(method, buf, fmin(method_len, METHOD_SIZE));

  char *path_start = method_divider + 1;
  char *path_divider = strstr(path_start, " ");
  int path_len = path_divider - path_start + 1;
  strlcpy(path, path_start, fmin(path_len, REQUEST_LINE_SIZE));

  // Body starts right after the headers end with a blank line
  char *body_start = strstr(buf, "\r\n\r\n") + 4;
  strlcpy(body, body_start, BODY_SIZE);

  handle_request(client, method, path, body);
}

void set_request_cb(void (*user_cb)(WiFiClient, const char *, const char *, const char *)) {
  request_cb = user_cb;
}

void handle_request(WiFiClient client, const char *method, const char *path, const char *body) {
  if (WIFI_DEBUG_REQUESTS) {
    debug_request(method, path, body); 
  }
  if (request_cb != NULL) {
    request_cb(client, method, path, body);
  }
}

void respond(WiFiClient client, ResponseCode code) {
  respond(client, code, NULL);
}

void respond(WiFiClient client, ResponseCode code, const char *body) {
  const char *head;
  if (code == RC_200) {
    head = HTTP_1_1 RC_TEXT_200 END_HEAD;
  } else if (code == RC_204) {
    head = HTTP_1_1 RC_TEXT_204 END_HEAD;
  } else if (code == RC_404) {
    head = HTTP_1_1 RC_TEXT_404 END_HEAD;
  } else if (code == RC_405) {
    head = HTTP_1_1 RC_TEXT_405 END_HEAD;
  } else if (code == RC_500) {
    head = HTTP_1_1 RC_TEXT_500 END_HEAD;
  }
  if (WIFI_DEBUG_REQUESTS) {
    Serial.print("Sending response: ");
    Serial.println(head);
  }
  client.write(head);

  if (body != NULL) {
    client.write(body);
  }
  client.flush();
}

void debug_request(const char *method, const char *path, const char *body) {
  Serial.println("Method:");
  Serial.println(method);
  Serial.println();
  Serial.println("Path:");
  Serial.println(path);
  Serial.println();

  Serial.println("Body:");
  Serial.println(body);
  Serial.println();
}

#endif
