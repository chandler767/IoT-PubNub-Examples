// PubNub example using ESP8266.

#include <ESP8266WiFi.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>

static char ssid[] = "YOUR_NETWORK_SSID";
static char pass[] = "YOUR_NETWORK_PASSWORD";

const static char pubkey[]  = "YOUR_PUB_KEY_HERE";
const static char subkey[]  = "YOUR_SUB_KEY_HERE";
const static char channel[] = "hello_world";

String message;

void setup() {
    Serial.begin(9600);
    Serial.println("Attempting to connect...");
    WiFi.begin(ssid, pass);
    if(WiFi.waitForConnectResult() != WL_CONNECTED) { // Connect to WiFi.
        Serial.println("Couldn't connect to WiFi.");
        while(1) delay(100);
    }
    else {
        Serial.print("Connected to SSID: ");
        Serial.println(ssid);
        PubNub.begin(pubkey, subkey); // Start PubNub.
        Serial.println("PubNub is set up.");
    }
}

void loop() {
    { // Subscribe.
        PubSubClient* sclient = PubNub.subscribe(channel); // Subscribe for messages.
        if (0 == sclient) { 
            Serial.println("Error subscribing to channel.");
            delay(1000);
            return;
        }
        while (sclient->wait_for_data()) { // Print messages.
            Serial.write(sclient->read());
        }
        sclient->stop();
    }
    { // Publish.
        char msg[] = "\"Hello world\"";
        WiFiClient* client = PubNub.publish(channel, msg); // Publish message to channel.
        if (0 == client) {
            Serial.println("Error publishing message.");
            delay(1000);
            return;
        }
        client->stop();
    }
    delay(1000);
}
