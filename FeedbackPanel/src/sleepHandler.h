#ifndef SLEEPHANDLER_H
#define SLEEPHANDLER_H
#include <Arduino.h>
#include "buttonHandler.h"

class SleepHandler {
    private:
        ButtonHandler** btns;
        void enableWakeUpListeners();
    public: 
        SleepHandler(ButtonHandler* btns[]);
        void goToSleep();
};

#endif