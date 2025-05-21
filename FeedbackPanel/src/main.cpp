#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include "buttonHandler.h"
// #define LED_GREEN 13

#define LED_GREEN 13
#define LED_BLUE 12
#define LED_YELLOW 14
#define LED_RED 27

#define BTN_GREEN 4
#define BTN_BLUE 5
#define BTN_YELLOW 18 
#define BTN_RED 23

#define WIFI_SSID "IoT_H3/4"
#define WIFI_PASSWORD "98806829"

// NTP server
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

int debounceTime = 50; // debounce time in milliseconds
int buttonCooldown = 7000; // 7 seconds cooldown time

int coolDownStart = 0; // variable to store the start time of the cooldown
int lastButtonPress = 0; // variable to store the last button press time

ButtonHandler green_btn(LED_GREEN, BTN_GREEN);
ButtonHandler blue_btn(LED_BLUE, BTN_BLUE);
ButtonHandler yellow_btn(LED_YELLOW, BTN_YELLOW);
ButtonHandler red_btn(LED_RED, BTN_RED);

void setup() {
    Serial.begin(115200); // output serial to the serial monitor in terminal
    green_btn.setup();
    blue_btn.setup();
    yellow_btn.setup();
    red_btn.setup();

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

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    
}

void loop() {


    // if(coolDownStart > 0 && (millis() - coolDownStart) >= buttonCooldown) {
    //     coolDownStart = 0; // reset cooldown
    // }else{
    //     return; // exit loop if cooldown is active
    // }
    green_btn.handleButtonPress();
    blue_btn.handleButtonPress();
    yellow_btn.handleButtonPress();
    red_btn.handleButtonPress();       
    // if (digitalRead(BTN_GREEN) == LOW){
    //     Serial.println("Button pressed");
    //     digitalWrite(LED_GREEN, HIGH);
    //     coolDownStart = millis(); // start cooldown
    //     delay(1000);
    //     digitalWrite(LED_GREEN, LOW);
    // }
    // if( digitalRead(BTN_BLUE) == LOW) {
    //     Serial.println("Button pressed");
    //     digitalWrite(LED_BLUE, HIGH);
    //     coolDownStart = millis(); // start cooldown
    //     delay(1000);
    //     digitalWrite(LED_BLUE, LOW);
    // }
    // if( digitalRead(BTN_YELLOW) == LOW) {
    //     Serial.println("Button pressed");
    //     digitalWrite(LED_YELLOW, HIGH);
    //     coolDownStart = millis(); // start cooldown
    //     delay(1000);
    //     digitalWrite(LED_YELLOW, LOW);
    // }
    // if( digitalRead(BTN_RED) == LOW) {
    //     Serial.println("Button pressed");
    //     digitalWrite(LED_RED, HIGH);
    //     coolDownStart = millis(); // start cooldown
    //     delay(1000);
    //     digitalWrite(LED_RED, LOW);
    // }


}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
    Serial.print("Current time: ");
    Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

