#include <math.h>

const int REQUEST_LINE_SIZE = 256;
const int BODY_SIZE = 1024;

void parse_headers(char *buf);
void handle_request(char *request_line, char *body);
void debug_request(char *request_line, char *body);

void parse_request(char *buf) {
  char request_line[REQUEST_LINE_SIZE];
  char body[BODY_SIZE];

  // Headers start right after the request line
  char *request_line_end = strstr(buf, "\r\n");
  // Add 1 byte for the terminator
  int request_line_len = request_line_end - buf + 1;
  strlcpy(request_line, buf, fmin(request_line_len, REQUEST_LINE_SIZE));

  // Body starts right after the headers end with a blank line
  char *body_start = strstr(buf, "\r\n\r\n") + 4;
  strlcpy(body, body_start, BODY_SIZE);

  handle_request(request_line, body);
}

void handle_request(char *request_line, char *body) {
  if (WIFI_DEBUG_REQUESTS) {
    debug_request(request_line, body); 
  }
  scroll_once(body);
}

void debug_request(char *request_line, char *body) {
  Serial.println("Request line:");
  Serial.println(request_line);
  Serial.println();

  Serial.println("Body:");
  Serial.println(body);
  Serial.println();
}
