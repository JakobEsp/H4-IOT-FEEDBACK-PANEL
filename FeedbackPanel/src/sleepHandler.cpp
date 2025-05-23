#include "sleepHandler.h"


SleepHandler::SleepHandler(ButtonHandler* btns[]) {
    this->btns = btns;
}

void SleepHandler::goToSleep() {
    this->enableWakeUpListeners();
    Serial.println("Going to sleep...");
    esp_deep_sleep_start();
}

void SleepHandler::enableWakeUpListeners() {
    Serial.println("Enabling wakeup listeners...");
    for(int i = 0; i < sizeof(btns)/sizeof(btns[0]); i++){
        btns[i]->turnOffLED(); // turn off all LEDs
        btns[i]->enableWakeUpListener(); // enable touch wakeup
    }   
}

