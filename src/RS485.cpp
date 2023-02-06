#include "RS485.h"

RS485::RS485(Logger* logger, uint8_t dePin) {
    _logger = logger;
    _dePin = dePin;
    _cmdBufferPos = 0;
}

void RS485::begin(uint32_t baudRate, SerialConfig mode) {
    Serial.begin(baudRate, mode);
    pinMode(_dePin, OUTPUT);
    digitalWrite(_dePin, LOW);
    _transmitting = false;
    _logger->log("RS485 initialized");
}

void RS485::loop() {
    while (Serial.available() > 0) {
        char nextChar = Serial.read();
        lastCharReceived = millis();

        _cmdBuffer[_cmdBufferPos] = nextChar;
        _cmdBufferPos++;
        if (_cmdBufferPos >= sizeof(_cmdBuffer)) {
            _logger->log("RS485 buffer overflow detected");
            _cmdBuffer[sizeof(_cmdBuffer)-1] = 0;
        }
    }

    if (_cmdBufferPos > 0 && millis() - lastCharReceived > TIMEOUT_MILLIS) {
        processCmdBuffer();
        _cmdBufferPos = 0;
    }
}

void RS485::processCmdBuffer() {
    logBuffer();
    if (_cmdBuffer[0] != 0x7E || _cmdBuffer[_cmdBufferPos-1] != 0x0D) {
        // TODO
    }
}

void RS485::logBuffer() {
    String result = "[";
    char buf[5];
    for (int p = 0; p < _cmdBufferPos; p++) {
        sprintf(buf, "%02X", _cmdBuffer[p]);
        result += buf;
        if (p < _cmdBufferPos - 1) {
            result += ", ";
        }
    }
    result += "]";
    _logger->log(result.c_str());
}

void RS485::sendCommand(char* destination, char* cmd) {
    loop();
    beginTransmission();
    // TODO, legacy from the RS485 server
    Serial.printf("%s:%s", destination, cmd);
    endTransmission();
} 

void RS485::beginTransmission() {
    digitalWrite(_dePin, HIGH);
    delayMicroseconds(_preDelay);
    _transmitting = true;
}

void RS485::endTransmission() {
    Serial.flush();
    delayMicroseconds(_postDelay);
    digitalWrite(_dePin, LOW);
    _transmitting = false;
}
