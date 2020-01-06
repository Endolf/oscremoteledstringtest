#include <Arduino.h>
#include "main.h"
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

static const char buildInfoLogString[] = "Build data: %s, %s %s";

const char ssid[] = "OSCNet";
const char password[] = "oscnet1324";

const IPAddress serverIp(192,168,4,1);
const unsigned int serverPort = 2000;
const unsigned int localPort = 2010;

char logBuffer[256];

WiFiUDP Udp;
OSCErrorCode error;

unsigned long lastOSCMessageTime = 0;

void setup()
{
    Serial.begin(115200);
    sprintf(logBuffer, buildInfoLogString, PIO_GIT_REV, __DATE__, __TIME__);
    Serial.println(logBuffer);
}

void ensureWiFiConnected() {
    if(WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to WiFi");
        WiFi.begin(ssid, password);
        while(WiFi.status()!=WL_CONNECTED) {
            Serial.print(".");
            delay(500);
        }
        Serial.println();
        Serial.println("WiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Udp.begin(localPort);
    }
}

void sendOSCMessage(OSCMessage &msg) {
    Udp.beginPacket(serverIp, serverPort);
    msg.send(Udp);
    Udp.endPacket();
}

void setSolidColourMessage(OSCMessage &msg) {
    msg.empty();
    msg.setAddress("/OSCTest/solidColour");
    msg.add(0x99FF22);
}

// msg.dispatch("/*/fire", setFireState);
// msg.dispatch("/*/rainbow", setRainbowState);
// msg.dispatch("/*/juggle", setJuggleState);
// msg.dispatch("/*/sinelon", setSinelonState);

void loop()
{
    ensureWiFiConnected();

    if(millis()-lastOSCMessageTime > 10000) {
        Serial.println("Sending solid colour request");
        OSCMessage msg("/");
        setSolidColourMessage(msg);
        sendOSCMessage(msg);
        Serial.println("Sent solid colour request");
        lastOSCMessageTime = millis();
    }
}