#pragma once

#include "Arduino.h"

class Battery {
    public:
        void begin();
        void loop();

        void processFrame(uint8_t* frame, uint16_t size);
        float getCellVoltage(uint8_t index);
        float getCellTemp(uint8_t index);
        float getEnvTemp();
        float getBMSTemp();
        float getCurrent();
        float getVoltage();
        float getPower();
        float getSOC();

    protected:
        void logFrame(uint8_t* frame, uint16_t size, uint8_t code);
        bool extractPayload(uint8_t frame[], uint16_t size, uint8_t result[]);
        int16_t toDecimal(uint8_t data[], uint8_t pos);
        float to1pDecimal(uint8_t data[], uint8_t pos);
        float to2pDecimal(uint8_t data[], uint8_t pos);
        float to3pDecimal(uint8_t data[], uint8_t pos);
        float toTemp(uint8_t data[], uint8_t pos);

    private:
        float cellVoltage[16];
        float cellTemp[4];
        float bmsTemp;
        float envTemp;
        float current;
        float voltage;
        float soc;
        uint32_t lastDataRequest;
};
