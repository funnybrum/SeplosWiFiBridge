#pragma once

#include "Arduino.h"

enum BalancerMode {
    ON,
    OFF,
    AUTO
};

class Balancer {
    public:
        void begin();
        void loop();
        void setMode(BalancerMode mode);
        void setThresholdVoltage(int mV);
        void setVoltageDifference(int mV);
        bool isBalancing();

    private:
        BalancerMode mode;
        bool isOn;
        int thresholdVoltage;
        int thresholdVoltageDiff;
};
