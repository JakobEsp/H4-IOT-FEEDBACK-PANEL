#include "timeManager.h"

TimeHandler::TimeHandler() {
    ntpServer = "pool.ntp.org";
    gmtOffset_sec = 3600;  // GMT+1
    daylightOffset_sec = 3600;
    isInitialized = false;
}

void TimeHandler::configure(const char* server, long gmtOffset, int daylightOffset) {
    ntpServer = server;
    gmtOffset_sec = gmtOffset;
    daylightOffset_sec = daylightOffset;
}

void TimeHandler::begin() {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    isInitialized = true;
}

void TimeHandler::printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.print("Current time: ");
    Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.println();
}

String TimeHandler::getTimeString() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return "Failed to obtain time";
    }
    
    char timeStringBuff[50];
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    return String(timeStringBuff);
}

bool TimeHandler::getLocalTime(struct tm* timeInfo) {
    if (!isInitialized) {
        Serial.println("Time not initialized! Call begin() first");
        return false;
    }
    return ::getLocalTime(timeInfo);
}