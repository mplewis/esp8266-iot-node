const int REQUEST_LINE_SIZE = 256;
const char HEADER_COUNT = 64;

char *request_line;
char *headers[HEADER_COUNT] = {0};
char *body;

void parse_headers(char *buf);
void print_request(char *request_line, char *headers[],
                   int header_count, char *body);

void parse_request(char *buf) {
  char *headers_end = strstr(buf, "\r\n\r\n");
  body = headers_end + 4;
  headers_end[0] = 0;  // Terminate headers
  parse_headers(buf);
}

void parse_headers(char *buf) {
  // parse http headers, tokenize by \r\n
  const char *header_sep = "\r\n";
  bool is_request_line = true;
  int header_num = 0;

  char *token = strtok(buf, header_sep);
  while (token != 0 && header_num < HEADER_COUNT) {
    if (is_request_line) {
      request_line = token;
      is_request_line = false;
    } else {
      headers[header_num++] = token;
    }
    token = strtok(0, header_sep);
  }

  print_request(request_line, headers, header_num, body);
}

void print_request(char *request_line, char *headers[],
                   int header_count, char *body) {
  Serial.println("Request line:");
  Serial.println(request_line);
  Serial.println();

  Serial.println("Headers:");
  for (int i = 0; i < header_count; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(headers[i]);
  }
  Serial.println();

  Serial.println("Body:");
  Serial.println(body);
  Serial.println();
}
