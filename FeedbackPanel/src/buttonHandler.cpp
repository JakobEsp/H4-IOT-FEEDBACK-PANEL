#include "buttonHandler.h"

class ButtonHandler{

    private:
        int ledPin;
        int buttonPin;
        int debounceTime;
        unsigned long lastDebounceTime = 0; // variable to store the last debounce time
        unsigned long lastButtonPress = 0; // variable to store the last button press time

    public:
        ButtonHandler(int ledPin, int buttonPin, int debounceTime, int cooldownTime){
            this->ledPin = ledPin;
            this->buttonPin = buttonPin;
            this->debounceTime = debounceTime;
            pinMode(ledPin, OUTPUT);
            pinMode(buttonPin, INPUT_PULLUP);
        }
};