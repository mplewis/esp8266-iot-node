#include <PubSubClient.h>

WiFiClient _wifi;
PubSubClient mqtt(_wifi);

void connect_mqtt();

void start_mqtt() {
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


  while (!mqtt.connected()) {
    static bool blink = HIGH;
  
    mqtt.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASSWORD);
    char state = mqtt.state();
    if (state == MQTT_CONNECTED) {
      break;
    }

    Serial.print("MQTT not connected. State: ");
    Serial.println(state, DEC);
    delay(1000);
    digitalWrite(BLINK_PIN, blink);
    blink = !blink;
  }

  digitalWrite(BLINK_PIN, LOW);
  Serial.println("MQTT ready.");
}

void publish(const char *payload) {
  mqtt.publish(MQTT_TOPIC_OUT, payload);
}
