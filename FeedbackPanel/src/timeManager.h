#ifndef TIMEHANDLER_H
#define TIMEHANDLER_H

#include <Arduino.h>
#include "time.h"

class TimeHandler {
private:
    const char* ntpServer;
    long gmtOffset_sec;
    int daylightOffset_sec;
    bool isInitialized;

public:
    // Default constructor
    TimeHandler();
    
    // Initialize time settings
    void configure(const char* server = "pool.ntp.org", long gmtOffset = 3600, int daylightOffset = 3600);
    
    // Initialize time with NTP server
    void begin();
    
    // Print the current time to Serial
    void printLocalTime();
    
    // // Get the current time as a string
    // String getTimeString();
    
    // // Check if time was successfully obtained
    // bool getLocalTime(struct tm* timeInfo);
};

#endif