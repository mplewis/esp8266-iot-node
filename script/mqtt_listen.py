#!/usr/bin/env python3

import paho.mqtt.client as mqtt

host = 'localhost'
port = 1883

def on_connect(client, userdata, rc):
    print('Connected: {}'.format(rc))
    client.subscribe('esp_out')

def on_message(client, userdata, msg):
    print('{}: {}'.format(msg.topic, msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(host, port)
client.loop_forever()
