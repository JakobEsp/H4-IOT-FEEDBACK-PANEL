#include <Arduino.h>

// #define LED_GREEN 13

#define LED_GREEN 13
#define LED_BLUE 12
#define LED_YELLOW 14
#define LED_RED 27

#define BTN_GREEN 4
#define BTN_BLUE 5
#define BTN_YELLOW 18 
#define BTN_RED 23


// put function declarations here:
int myFunction(int, int);

void setup() {
    Serial.begin(115200);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(BTN_GREEN, INPUT_PULLUP);
    pinMode(BTN_BLUE, INPUT_PULLUP);
    pinMode(BTN_YELLOW, INPUT_PULLUP);
    pinMode(BTN_RED, INPUT_PULLUP);

}

void loop() {
       
    if (digitalRead(BTN_GREEN) == LOW) {
        Serial.println("Button pressed");
        digitalWrite(LED_GREEN, HIGH);
        delay(1000);
        digitalWrite(LED_GREEN, LOW);
    }
    if( digitalRead(BTN_BLUE) == LOW) {
        Serial.println("Button pressed");
        digitalWrite(LED_BLUE, HIGH);
        delay(1000);
        digitalWrite(LED_BLUE, LOW);
    }
    if( digitalRead(BTN_YELLOW) == LOW) {
        Serial.println("Button pressed");
        digitalWrite(LED_YELLOW, HIGH);
        delay(1000);
        digitalWrite(LED_YELLOW, LOW);
    }
    if( digitalRead(BTN_RED) == LOW) {
        Serial.println("Button pressed");
        digitalWrite(LED_RED, HIGH);
        delay(1000);
        digitalWrite(LED_RED, LOW);
    }

  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}