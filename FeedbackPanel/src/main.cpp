#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "buttonHandler.h"
#include "timeManager.h"

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

TimeHandler timeHandler;

int debounceTime = 50; // debounce time in milliseconds
int buttonCooldown = 7000; // 7 seconds cooldown time

int coolDownStart = 0; // variable to store the start time of the cooldown
int lastButtonPress = 0; // variable to store the last button press time



ButtonHandler green_btn(LED_GREEN, BTN_GREEN, TOUCH_BTN_GREEN, startCoolDown);
ButtonHandler blue_btn(LED_BLUE, BTN_BLUE, TOUCH_BTN_BLUE, startCoolDown);
ButtonHandler yellow_btn(LED_YELLOW, BTN_YELLOW, TOUCH_BTN_YELLOW, startCoolDown);
ButtonHandler red_btn(LED_RED, BTN_RED, TOUCH_BTN_RED, startCoolDown);


ButtonHandler* btns[] = {&green_btn, &blue_btn, &yellow_btn, &red_btn};

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
                btns[i]->handleButtonPress(); // enable touch wakeup
            }
        }
    }
    Serial.print("Touch Pin: ");
    Serial.println(touchPin);

    // Connecting to Wi-Fi
    Serial.println("Connecting to");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println(".");
    }
    
    // Connected to Wi-Fi
    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    
    // Initialize the timeHandler
    timeHandler.begin();
    timeHandler.printLocalTime();
   
}

void loop() {
    if(coolDownStart > 0 && (millis() - coolDownStart) >= buttonCooldown) {
        coolDownStart = 0; // reset cooldown
        Serial.println("Cooldown ended");
        for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
            btns[i]->turnOffLED(); // turn off all LEDs
            btns[i]->enableWakeUpListener(); // enable touch wakeup
        }   
        Serial.println("Going sleep"); 
        esp_deep_sleep_start(); 
    }else if(
        coolDownStart > 0 && (millis() - coolDownStart) < buttonCooldown
    ){
        return;
    }
    
    green_btn.handleButtonPress();
    blue_btn.handleButtonPress();
    yellow_btn.handleButtonPress();
    red_btn.handleButtonPress(); 
}

void startCoolDown(){
    coolDownStart = millis(); // start cooldown
    for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
        btns[i]->clearButtonState(); // turn off all LEDs
    } 
    Serial.println("Cooldown started");
}
