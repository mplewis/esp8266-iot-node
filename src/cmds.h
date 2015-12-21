const char BUF_LEN = 64;
const char ARG_COUNT = 16;
char buf[BUF_LEN] = {0};
char *args[ARG_COUNT] = {0};

void _mqtt_in(char *topic, uint8_t *payload, unsigned int len);
void args_in();
void publish(const char *payload);  // mqtt.h
unsigned long board_id();  // mqtt.h

void start_cmds() {
  mqtt.setCallback(_mqtt_in);
}

void args_in() {
  char *cmd = args[0];
  if (strcasecmp(cmd, "ident") == 0) {
    String reply = "";
    reply += "IDENT,";
    reply += board_id();
    reply += ",";
    reply += WiFi.localIP().toString();
    reply += ",";
    reply += BOARD_NAME;
    reply += ",";
    reply += BOARD_DESC;
    reply.toCharArray(buf, BUF_LEN);
    publish(buf);
  }
}

void _mqtt_in(char *topic, uint8_t *payload, unsigned int len) {
  // copy bytes from payload to buf and terminate
  for (char i = 0; i <= len; i++) {
    if (i == BUF_LEN - 1 || i == len) {
      buf[i] = 0;
      break;
    }
    buf[i] = payload[i];
  }

  // tokenize by commas
  char *token = strtok(buf, ",");
  int arg_num = 0;
  while (token != 0 && arg_num < ARG_COUNT) {
    args[arg_num++] = token;
    token = strtok(0, ",");
  }

  // clear unused args in the array
  for (char i = arg_num; i < ARG_COUNT; i++) {
    args[i] = NULL;
  }

  // handle parsed arguments
  args_in();

  blink(100);
}
