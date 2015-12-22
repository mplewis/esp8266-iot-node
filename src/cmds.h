const char HEADER_COUNT = 64;
char *headers[HEADER_COUNT] = {0};
const char *body_sep = "\r\n\r\n";
const char *header_sep = "\r\n";

void parse_headers(char *buf);

void parse_request(char *buf) {
  char *raw_headers = strtok(buf, body_sep);
  char *body = strtok(0, body_sep);
  Serial.println("Headers:");
  Serial.println(raw_headers);
  Serial.println("Body:");
  Serial.println(body);
  parse_headers(raw_headers);
}

void parse_headers(char *buf) {
  // parse http headers, tokenize by \r\n
  char *token = strtok(buf, header_sep);
  int header_num = 0;
  while (token != 0 && header_num < HEADER_COUNT) {
    headers[header_num++] = token;
    token = strtok(0, header_sep);
  }

  // clear unused headers in the array
  for (int i = header_num; i < HEADER_COUNT; i++) {
    headers[i] = NULL;
  }

  for (int i = 0; i < header_num; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(headers[i]);
  }
}
