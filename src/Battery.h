#pragma once

#include "Arduino.h"

class Battery {
    public:
        void processFrame(uint8_t* frame, uint16_t size);
        float* getCellVoltages();
        float* getCellTemp();
        float getEnvTemp();
        float getBMSTemp();
        float getCurrent();
        float getVoltage();
        float getPower();

    protected:
        void logFrame(uint8_t* frame, uint16_t size, uint8_t code);

    private:
        float cellVoltage[16];
        float cellTemp[4];
        float bmsTemp;
        float envTemp;
        float current;
        float voltage;
};
