#include "buttonHandler.h"

#define DEBOUNCE_TIME 100 // milliseconds

void ButtonHandler::setup(){
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
}

ButtonHandler::ButtonHandler(int ledPin, int buttonPin){
    this->ledPin = ledPin;
    this->buttonPin = buttonPin;
    this->debounceTime = DEBOUNCE_TIME;
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP);
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
        lastDebounceTime = 0; // Update the last button press time
    }
}

