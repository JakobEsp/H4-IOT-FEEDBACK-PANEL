#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "timeManager.h"
#include "buttonHandler.h"
#include "networkHandler.h"

#define MQTT_SERVER "192.168.0.130"
#define MQTT_PORT 1883
#define MQTT_USERNAME "device11"      
#define MQTT_PASSWORD "device11-password"
#define MQTT_TOPIC "sensor/device11"

extern TimeHandler timeHandler; 
class MqttHandler {
    private:
        PubSubClient* client;
        void reconnect();
    public:
        MqttHandler(WiFiClient* wifiClient);
        void sendResult(ButtonHandler* button);
};
#endif