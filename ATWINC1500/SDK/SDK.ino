/*
  PubNub example using ATWINC1500.
*/


#include <WiFi101.h>
#define PubNub_BASE_CLIENT WiFiClient
#include <PubNub.h>

static char ssid[] = "pubnub-visitor"; // SSID
static char pass[] = "data-stream!"; // Password

const static char pubkey[]  = "pub-c-5e98b32b-48a6-4616-881f-4e3770ed58eb";
const static char subkey[]  = "sub-c-0a38b39c-7373-11e9-912a-e2e853b4b660";
const static char channel[] = "hello_world";

String message;

void setup()
{
    Serial.begin(9600);

    // Connect to WiFi
    Serial.println("Attempting to connect...");
    if(WiFi.begin(ssid, pass) != WL_CONNECTED) {
        Serial.println("Couldn't connect to WiFi.");
        while(1) delay(100);
    }
    else {
        Serial.print("Connected to SSID: ");
        Serial.println(ssid);
        PubNub.begin(pubkey, subkey); // When connected start PubNub.
        Serial.println("PubNub is set up.");
    }
}


void loop()
{

    /* Subscribe */ 
    {
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

   /* Publish */
   {
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
