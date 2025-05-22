#include "networkHandler.h"


#define WIFI_SSID "IoT_H3/4"
#define WIFI_PASSWORD "98806829"

void NetworkHandler::connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

}

void NetworkHandler::disconnect() {
    WiFi.disconnect();
    Serial.println("Disconnected from WiFi");
}