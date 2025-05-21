#include "buttonHandler.h"

#define DEBOUNCE_TIME 500 // milliseconds
#define THRESHOLD 10 // threshold for touch wakeup
void ButtonHandler::setup(){
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

ButtonHandler::ButtonHandler(int ledPin, int buttonPin, void (*callback)()){
    this->ledPin = ledPin;
    this->buttonPin = buttonPin;
    this->debounceTime = DEBOUNCE_TIME;
    this->callback = callback;
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    // touchSleepWakeUpEnable(buttonPin, THRESHOLD);

}

void ButtonHandler::handleButtonPress(){
    int buttonState = digitalRead(buttonPin);
    unsigned long currentTime = millis();

    if (buttonState == LOW) {
        // Check if the button is pressed for more than the debounce time     
        lastDebounceTime = currentTime; // Update the last debounce time
    } 
    if ((currentTime - lastDebounceTime) > debounceTime && lastDebounceTime > 0) {
        digitalWrite(ledPin, HIGH); // Turn on the LED
        Serial.println("Button Pressed");
        callback(); // Call the callback function
        lastDebounceTime = 0; // Update the last button press time
    }
}

void ButtonHandler::turnOffLED(){
    digitalWrite(ledPin, LOW); // Turn off the LED
}   

