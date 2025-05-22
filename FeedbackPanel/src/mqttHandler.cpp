#include "mqttHandler.h"

// Static member initialization
MqttHandler* MqttHandler::instance = nullptr;

// Static callback function
void MqttHandler::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (instance) {
        // Convert payload to string
        String message;
        for (unsigned int i = 0; i < length; i++) {
            message += (char)payload[i];
        }
        
        // Forward to instance method
        instance->handleMessage(String(topic), message);
    }
}

MqttHandler::MqttHandler(TimeHandler* timeHandlerRef) {
    timeHandler = timeHandlerRef;
    wifiClient = new WiFiClient();
    mqttClient = new PubSubClient(*wifiClient);
    messageCallback = nullptr;
    isConnected = false;
    
    // Set static instance pointer
    instance = this;
}

MqttHandler::~MqttHandler() {
    if (mqttClient) {
        if (mqttClient->connected()) {
            mqttClient->disconnect();
        }
        delete mqttClient;
    }
    
    if (wifiClient) {
        delete wifiClient;
    }
    
    if (instance == this) {
        instance = nullptr;
    }
}

void MqttHandler::configure(const char* server, int port, const char* username, 
                           const char* password, const char* clientId, 
                           const char* topicPrefix, const char* rootCACert) {
    mqttServer = server;
    mqttPort = port;
    mqttUser = username;
    mqttPassword = password;
    mqttClientId = clientId;
    mqttTopicPrefix = topicPrefix;
    rootCA = rootCACert;
    
    
    // Configure MQTT client
    mqttClient->setServer(mqttServer, mqttPort);
    mqttClient->setCallback(mqttCallback);
}

bool MqttHandler::begin() {
    return connect();
}

bool MqttHandler::connect() {
    int attempts = 0;
    
    while (!mqttClient->connected() && attempts < 5) {
        Serial.println("Connecting to MQTT...");
        
        if (mqttClient->connect(mqttClientId, mqttUser, mqttPassword)) {
            Serial.println("Connected to MQTT broker");
            isConnected = true;
            
            // Resubscribe to topics (in case of reconnection)
            // Example: subscribe(String(mqttTopicPrefix + "commands").c_str());
            
            return true;
        } else {
            Serial.print("Failed to connect to MQTT, rc=");
            Serial.print(mqttClient->state());
            Serial.println(" Retrying in 2 seconds");
            delay(2000);
            attempts++;
        }
    }
    
    isConnected = false;
    return false;
}

bool MqttHandler::sendButtonPress(const char* buttonColor, int buttonValue) {
    // Check if MQTT is connected, reconnect if needed
    if (!mqttClient->connected()) {
        if (!connect()) {
            return false;
        }
    }
    
    // Create a JSON document
    StaticJsonDocument<200> doc;
    doc["device_id"] = this->mqttClientId;  // Changed to use this pointer
    doc["button"] = buttonColor;
    doc["value"] = buttonValue;
    doc["timestamp"] = timeHandler->getTimeString();
    
    // Convert to string
    char jsonBuffer[256];
    serializeJson(doc, jsonBuffer);
    
    // Create topic 
    String topic = String(mqttTopicPrefix) + buttonColor;
    
    // Publish message
    bool success = mqttClient->publish(topic.c_str(), jsonBuffer);
    
    if (success) {
        Serial.println("Button press data sent to MQTT");
    } else {
        Serial.println("Failed to send button press data");
    }
    
    return success;
}

bool MqttHandler::subscribe(const char* topic) {
    if (!mqttClient->connected()) {
        if (!connect()) {
            return false;
        }
    }
    
    bool result = mqttClient->subscribe(topic);
    if (result) {
        Serial.print("Subscribed to topic: ");
        Serial.println(topic);
    } else {
        Serial.print("Failed to subscribe to topic: ");
        Serial.println(topic);
    }
    
    return result;
}

bool MqttHandler::unsubscribe(const char* topic) {
    if (!mqttClient->connected()) {
        return false;
    }
    
    return mqttClient->unsubscribe(topic);
}

void MqttHandler::setCallback(MqttMessageCallback callback) {
    messageCallback = callback;
}

void MqttHandler::handleMessage(String topic, String message) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    Serial.println(message);
    
    if (messageCallback) {
        messageCallback(topic, message);
    }
}

void MqttHandler::maintainConnection() {
    if (!mqttClient->connected()) {
        connect();
    }
}

void MqttHandler::loop() {
    if (mqttClient->connected()) {
        mqttClient->loop();
    } else {
        // Try to reconnect
        maintainConnection();
    }
}