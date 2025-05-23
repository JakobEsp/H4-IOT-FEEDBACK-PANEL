#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H
#include <WiFi.h>

extern WiFiClient wifiClient;

class NetworkHandler {
    public:
        void connect();
        void disconnect();
};
#endif