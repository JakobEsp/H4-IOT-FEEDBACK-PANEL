#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
private:
    int ledPin;
    int buttonPin;
    int touchPin;
    int debounceTime;
    int currentButtonState;
    int lastButtonState;
    int ledState = LOW;
    void (*callback)();
    unsigned long lastDebounceTime = 0;
    unsigned long lastButtonPress = 0;
public:
    ButtonHandler(int ledPin, int buttonPin,int touchPin, void (*callback)());
    void setup();
    void handleButtonPress();
    void turnOffLED();
    void clearButtonState();
    void enableWakeUpListener();
    int getButtonPin() { return buttonPin; }
    int getTouchPin() { return touchPin; }
};

#endif