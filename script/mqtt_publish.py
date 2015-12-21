#!/usr/bin/env python3

from sys import argv
import paho.mqtt.client as mqtt

host = 'localhost'
port = 1883

client = mqtt.Client()
client.connect(host, port)
client.publish('esp_in', argv[1])
