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
uint8_t message = 0;

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

void setSolidColourMessage(OSCMessage &msg, uint32_t colour) {
    msg.empty();
    msg.setAddress("/OSCTest/solidColour");
    msg.add(colour);
    sprintf(logBuffer, "Solid colour (0x%6.6X) message", colour);
    Serial.println(logBuffer);
}

void setFirePatternMessage(OSCMessage &msg) {
    msg.empty();
    msg.setAddress("/OSCTest/fire");
    Serial.println("Fire pattern message");
}

void setRainbowPatternMessage(OSCMessage &msg) {
    msg.empty();
    msg.setAddress("/OSCTest/rainbow");
    Serial.println("Rainbow pattern message");
}

void setJugglePatternMessage(OSCMessage &msg) {
    msg.empty();
    msg.setAddress("/OSCTest/juggle");
    Serial.println("Juggle pattern message");
}

void setSinelonPatternMessage(OSCMessage &msg) {
    msg.empty();
    msg.setAddress("/OSCTest/sinelon");
    Serial.println("Sinelon pattern message");
}

void loop()
{
    ensureWiFiConnected();

    if(millis()-lastOSCMessageTime > 10000) {
        Serial.println("Sending message");
        OSCMessage msg("/");

        switch (message)
        {
        case 0:
            setSolidColourMessage(msg, 0xFF0000);
            break;
        case 1:
            setSolidColourMessage(msg, 0xFFFF00);
            break;
        case 2:
            setSolidColourMessage(msg, 0x00FF00);
            break;
        case 3:
            setSolidColourMessage(msg, 0x00FFFF);
            break;
        case 4:
            setSolidColourMessage(msg, 0x0000FF);
            break;
        case 5:
            setSolidColourMessage(msg, 0xFF00FF);
            break;
        case 6:
            setFirePatternMessage(msg);
            break;
        case 7:
            setRainbowPatternMessage(msg);
            break;
        case 8:
            setJugglePatternMessage(msg);
            break;
        case 9:
            setSinelonPatternMessage(msg);
            break;
        default:
            setSolidColourMessage(msg, 0x100000);
            break;
        }

        sendOSCMessage(msg);
        Serial.println("Sent message");
        message++;
        message %= 10;
        lastOSCMessageTime = millis();
    }
}