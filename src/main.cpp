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

char logBuffer[256];

WiFiUDP Udp;
OSCErrorCode error;

void setup()
{
    Serial.begin(115200);
    sprintf(logBuffer, buildInfoLogString, PIO_GIT_REV, __DATE__, __TIME__);
    Serial.println(logBuffer);
}

void ensureWiFiConnected() {
    if(WiFi.status() != WL_CONNECTED) {

    }
}

void loop()
{
    ensureWiFiConnected();

    //OSCBundle bundle;
    OSCMessage msg;
    int size = Udp.parsePacket();

    if (size > 0)
    {
        while (size--)
        {
            msg.fill(Udp.read());
        }
        if (!msg.hasError())
        {
            // msg.dispatch("/*/solidColour", solidColour);
            // msg.dispatch("/*/solidColourRed", solidColourRed);
            // msg.dispatch("/*/solidColourGreen", solidColourGreen);
            // msg.dispatch("/*/solidColourBlue", solidColourBlue);
            // msg.dispatch("/*/fire", setFireState);
            // msg.dispatch("/*/rainbow", setRainbowState);
            // msg.dispatch("/*/juggle", setJuggleState);
            // msg.dispatch("/*/sinelon", setSinelonState);
            // msg.dispatch("/*/brightness", setBrightness);
        }
        else
        {
            error = msg.getError();
            Serial.print("error: ");
            Serial.println(error);
        }
    }
}