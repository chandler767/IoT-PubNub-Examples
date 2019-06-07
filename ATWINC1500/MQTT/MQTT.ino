/*
  PubNub MQTT example using ATWINC1500.
*/

#include <WiFi101.h>
#include <PubSubClient.h>

// Connection info.
const char* ssid = "YOUR_NETWORK_SSID";
const char* password =  "YOUR_NETWORK_PASSWORD";
const char* mqttServer = "mqtt.pndsn.com";
const int mqttPort = 1883;
const char* clientID = "YOUR_PUB_KEY_HERE/YOUR_SUB_KEY_HERE/CLIENT_ID";
const char* channelName = "hello_world";

WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

void callback(char* topic, byte* payload, unsigned int length) {
  String payload_buff;
  for (int i=0;i<length;i++) {
    payload_buff = payload_buff+String((char)payload[i]);
  }
  Serial.println(payload_buff); // Print out messages.
}

long lastReconnectAttempt = 0;

boolean reconnect() {
  if (client.connect(clientID)) {
    client.subscribe(channelName); // Subscribe to channel.
  }
  return client.connected();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect...");
  if(WiFi.begin(ssid, password) != WL_CONNECTED) { // Connect to WiFi.
      Serial.println("Couldn't connect to WiFi.");
      while(1) delay(100);
  }
  client.setServer(mqttServer, mqttPort); // Connect to PubNub.
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}

void loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) { // Try to reconnect.
      lastReconnectAttempt = now;
      if (reconnect()) { // Attempt to reconnect.
        lastReconnectAttempt = 0;
      }
    }
  } else { // Connected.
    client.loop();
    client.publish(channelName,"Hello world!"); // Publish message.
    delay(1000);
  }
}
