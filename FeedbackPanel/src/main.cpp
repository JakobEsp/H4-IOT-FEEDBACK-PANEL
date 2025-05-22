#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "buttonHandler.h"
#include "timeManager.h"
#include <PubSubClient.h>  
#include <ArduinoJson.h> 
#include "mqttHandler.h"

#define MQTT_SERVER "192.168.0.130"
#define MQTT_PORT 1883
#define MQTT_USERNAME "device11"      
#define MQTT_PASSWORD "device11-password"
#define MQTT_TOPIC "sensor/device11"


#define LED_GREEN 4
#define LED_BLUE 21
#define LED_YELLOW 19
#define LED_RED 18

#define BTN_GREEN 13
#define BTN_BLUE 12
#define BTN_YELLOW 14 
#define BTN_RED 27

#define TOUCH_BTN_GREEN 4
#define TOUCH_BTN_BLUE 5
#define TOUCH_BTN_YELLOW 6
#define TOUCH_BTN_RED 7

#define WIFI_SSID "IoT_H3/4"
#define WIFI_PASSWORD "98806829"
   
void startCoolDown();
void coolDownFinished();
void enableWakeUpListeners();

TimeHandler timeHandler;

int debounceTime = 50; // debounce time in milliseconds


int buttonCooldown = 7000; // 7 seconds cooldown time
int coolDownStart = 0; // variable to store the start time of the cooldown

ButtonHandler green_btn(LED_GREEN, BTN_GREEN, TOUCH_BTN_GREEN, startCoolDown);
ButtonHandler blue_btn(LED_BLUE, BTN_BLUE, TOUCH_BTN_BLUE, startCoolDown);
ButtonHandler yellow_btn(LED_YELLOW, BTN_YELLOW, TOUCH_BTN_YELLOW, startCoolDown);
ButtonHandler red_btn(LED_RED, BTN_RED, TOUCH_BTN_RED, startCoolDown);

ButtonHandler* btns[] = {&green_btn, &blue_btn, &yellow_btn, &red_btn};


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Create MQTT handler instance
MqttHandler mqttHandler(&timeHandler);


void setup() {
    Serial.begin(115200); // output serial to the serial monitor in terminal

    green_btn.setup();
    blue_btn.setup();
    yellow_btn.setup();
    red_btn.setup();

    touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();
    if (touchPin == TOUCH_PAD_MAX) {
        Serial.println("No touch pin detected");
    } else {
        for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
            if(btns[i]->getTouchPin() == touchPin){
                Serial.print("Touch Pin activiated ");
                Serial.println(touchPin);
                btns[i]->handleButtonPress();
                Serial.println("cooldown sharted");
                Serial.println(coolDownStart);
            }
        }
    }
    if(!coolDownStart){
        Serial.println("No Cooldown started Going to sleep");
        enableWakeUpListeners();
        esp_deep_sleep_start(); 
    }

    // Network call - Connect to WiFi and send MQTT message
    
    Serial.println("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
        // Initialize time from NTP
        timeHandler.begin();
        timeHandler.printLocalTime();
        
        // Send MQTT timestamp and button press 1- 4
        // INSERT THE CODE RIGHT HERE - replace these comment lines
        touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();
        int buttonNumber = 0;
        
        // Determine which button was pressed
        if (touchPin == TOUCH_BTN_GREEN) buttonNumber = 1;
        else if (touchPin == TOUCH_BTN_BLUE) buttonNumber = 2;
        else if (touchPin == TOUCH_BTN_YELLOW) buttonNumber = 3;
        else if (touchPin == TOUCH_BTN_RED) buttonNumber = 4;
        
        if (buttonNumber > 0) {
            // Connect to MQTT broker
            mqttHandler.configure(MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, 
                        MQTT_PASSWORD, "ESP32FeedbackPanel", 
                        MQTT_TOPIC, "");

            if (mqttHandler.begin()) {
                char buttonName[10];
                sprintf(buttonName, "button%d", buttonNumber);
                
                // Send button press (buttonName, buttonValue)
                mqttHandler.sendButtonPress(buttonName, buttonNumber);
                Serial.print("MQTT message sent for button: ");
                Serial.println(buttonNumber);
            }
            else {
                Serial.println("Failed to connect to MQTT broker");
            }
        }
        
        // Disconnect WiFi to save power
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }

    
    
    while(coolDownStart > 0 && (millis() - coolDownStart) < buttonCooldown){
        delay(50); // wait for cooldown to end
    }

    coolDownFinished();
}

void loop() {
}

void enableWakeUpListeners(){
    for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
        btns[i]->turnOffLED(); // turn off all LEDs
        btns[i]->enableWakeUpListener(); // enable touch wakeup
    }   
}

void coolDownFinished(){
    coolDownStart = 0; // reset cooldown
    Serial.println("Cooldown ended");
    enableWakeUpListeners();
    Serial.println("Going sleep"); 
    esp_deep_sleep_start(); 
}

void startCoolDown(){
    coolDownStart = millis(); // start cooldown
    for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
        btns[i]->clearButtonState(); // turn off all LEDs
    } 
    Serial.println("Cooldown started");
}