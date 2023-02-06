#pragma once

#include "Logger.h"

#define TIMEOUT_MILLIS 250

class RS485 {
    public:
        RS485(Logger* logger, uint8_t dePin = D1);
        void begin(uint32_t baudRate = 19200, SerialConfig mode = SERIAL_8N1);
        void loop();
        void processCmdBuffer();
        void logBuffer();

        void sendCommand(char* destination, char* cmd);

    protected:
        void beginTransmission();
        void endTransmission();

    private:
        Logger* _logger;

        uint8_t _dePin;
        uint16_t _preDelay;
        uint16_t _postDelay;
        bool _transmitting = false;
        
        char _cmdBuffer[256];
        uint16_t _cmdBufferPos;

        uint32_t lastCharReceived = 0;
};
