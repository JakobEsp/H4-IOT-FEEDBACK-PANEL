#include "mqttHandler.h"
#include "ArduinoJson.h"

TimeHandler timeHandler; // <-- This is the only definition

MqttHandler::MqttHandler(WiFiClient* wifiClient) {
    this->client = new PubSubClient(*wifiClient);
    this->client->setServer(MQTT_SERVER, MQTT_PORT);
}

void MqttHandler::reconnect() {
    while (!client->connected()) {
        Serial.println("Connecting to MQTT...");
        if (client->connect(MQTT_USERNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("Connected to MQTT broker");
        } else {
            Serial.print("Failed to connect to MQTT, rc=");
            Serial.print(client->state());
            Serial.println(" Retrying in 2 seconds");
            delay(2000);
        }
    }
}

void MqttHandler::sendResult(ButtonHandler* button) {

    if (!client->connected()) {
        reconnect();
    }
    timeHandler.begin();
    String time = timeHandler.getTimeString();
    StaticJsonDocument<128> result;
    result["feedback"] = button->getButtonPin();
    result["timestamp"] = time;
    char payload[128];
    serializeJson(result, payload);
    Serial.println("Sending result...");
    boolean succes = client->publish(MQTT_TOPIC, payload);
    if (succes) {
        Serial.println("Result sent successfully");
    } else {
        Serial.println("Failed to send result");
    }
}