#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "timeManager.h"

typedef void (*MqttMessageCallback)(String topic, String message);

class MqttHandler {
private:
    const char* mqttServer;
    int mqttPort;
    const char* mqttUsername;
    const char* mqttPassword;
    const char* mqttClientId;
    const char* mqttTopicPrefix;

    // SSL/TLS certificate
    const char* rootCA;

    // Client instances
    WiFiClientSecure* wifiClient;
    PubSubClient* mqttClient;

    // Reference to the time handler
    TimeHandler* timeHandler;

    // Callback for message handling
    MqttMessageCallback messageCallback;

    // Static callback
    static void mqttCallback(char* topic, byte* payload, unsigned int length);

    // Instance pointer for static callback
    static MqttHandler* instance;

    // Connection status
    bool isConnected;

    // Attempt to connect to the MQTT broker
    bool connect();

public:
    // Constructor
    MqttHandler(TimeHandler* timeHandlerRef);
    
    // Destructor
    ~MqttHandler();
    
    // Initialize with server settings
    void configure(const char* server, int port, const char* username, 
                 const char* password, const char* clientId, 
                 const char* topicPrefix, const char* rootCACert);
    
    // Connect to the MQTT broker
    bool begin();
    
    // Send button press data
    bool sendButtonPress(const char* buttonColor, int buttonValue);
    
    // Subscribe to a topic
    bool subscribe(const char* topic);
    
    // Unsubscribe from a topic
    bool unsubscribe(const char* topic);
    
    // Set message callback
    void setCallback(MqttMessageCallback callback);
    
    // Process received message
    void handleMessage(String topic, String message);
    
    // Check connection and reconnect if necessary
    void maintainConnection();
    
    // Process incoming messages (must be called regularly)
    void loop();


};
#endif