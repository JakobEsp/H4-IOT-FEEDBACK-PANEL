#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "buttonHandler.h"
#include "timeManager.h"
#include "networkHandler.h"
#include "mqttHandler.h"

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

#define DEBOUNCE_TIME 50 // debounce time in milliseconds
#define BUTTON_COOLDOWN 7000 // 7 seconds cooldown time

void goToSleep();
void startCoolDown();
void coolDownFinished();
void enableWakeUpListeners();

// TimeHandler timeHandler;
NetworkHandler networkHandler;
MqttHandler mqttHandler(&wifiClient);

int coolDownStart = 0; // variable to store the start time of the cooldown

ButtonHandler green_btn("Very Happy",LED_GREEN, BTN_GREEN, TOUCH_BTN_GREEN, startCoolDown);
ButtonHandler blue_btn("Moderately Happy",LED_BLUE, BTN_BLUE, TOUCH_BTN_BLUE, startCoolDown);
ButtonHandler yellow_btn("Not Satisfied",LED_YELLOW, BTN_YELLOW, TOUCH_BTN_YELLOW, startCoolDown);
ButtonHandler red_btn("Much Angry",LED_RED, BTN_RED, TOUCH_BTN_RED, startCoolDown);

ButtonHandler* btns[] = {&green_btn, &blue_btn, &yellow_btn, &red_btn};

void setup() {
    Serial.begin(115200); // output serial to the serial monitor in terminal

    green_btn.setup();
    blue_btn.setup();
    yellow_btn.setup();
    red_btn.setup();

    ButtonHandler* selectedButton = nullptr;
    touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();
    if (touchPin == TOUCH_PAD_MAX) {
        Serial.println("No touch pin detected");
        // Could also go to sleep here
    } else {
        for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
            if(btns[i]->getTouchPin() == touchPin){
                btns[i]->handleButtonPress();
                selectedButton = btns[i];
            }
        }
    }
    if(!coolDownStart){
        Serial.println("No Cooldown started Going to sleep");
        goToSleep();
    }

    // make newtork call here
    networkHandler.connect();

    mqttHandler.sendResult(selectedButton);

    networkHandler.disconnect();

    while(coolDownStart > 0 && (millis() - coolDownStart) < BUTTON_COOLDOWN){
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
    Serial.println("Going sleep"); 
    goToSleep();
}

void startCoolDown(){
    coolDownStart = millis(); // start cooldown
    for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
        btns[i]->clearButtonState(); // turn off all LEDs
    } 
    Serial.println("Cooldown started");
}

void goToSleep(){
    Serial.println("Going to sleep...");
    enableWakeUpListeners();
    esp_deep_sleep_start();
}   