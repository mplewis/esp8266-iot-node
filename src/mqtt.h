#include <PubSubClient.h>

#define BLINK_PIN_MQTT PIN_LED_MODULE

WiFiClient _wifi;
PubSubClient mqtt(_wifi);

void connect_mqtt();
bool _attempt_connect_once();

void start_mqtt() {
  start_leds();
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  connect_mqtt();
  mqtt.subscribe(MQTT_TOPIC_IN);
}

void connect_mqtt() {
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to ");
  Serial.print(MQTT_SERVER);
  Serial.println("...");

  if (!_attempt_connect_once()) {
    delay(1000);
  }
}

void maintain_mqtt() {
  connect_mqtt();
  mqtt.loop();
}

void publish(const char *payload) {
  mqtt.publish(MQTT_TOPIC_OUT, payload);
  blink(100);
}

bool _attempt_connect_once() {
  if (mqtt.connected()) {
    return true;
  }

  digitalWrite(BLINK_PIN_MQTT, HIGH);
  mqtt.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASSWORD);
  char state = mqtt.state();

  if (state == MQTT_CONNECTED) {
    Serial.println("MQTT ready.");
    digitalWrite(BLINK_PIN_MQTT, LOW);
    return true;

  } else {
    Serial.print("MQTT not connected. State: ");
    Serial.println(state, DEC);
    digitalWrite(BLINK_PIN_MQTT, LOW);
    return false;
  }
}
