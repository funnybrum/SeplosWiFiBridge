#include "Main.h"

RS485::RS485(Logger* logger, uint8_t dePin) {
    _logger = logger;
    _dePin = dePin;
    _cmdBufferPos = 0;
}

void RS485::begin(uint32_t baudRate, SerialConfig mode) {
    Serial.begin(baudRate);
    pinMode(_dePin, OUTPUT);
    digitalWrite(_dePin, LOW);
    _transmitting = false;
    _logger->log("RS485 initialized");
}

char buf[100];

void RS485::loop() {
    while (Serial.available() > 0) {
        digitalWrite(LED_BUILTIN, LOW);
        char nextChar = Serial.read();
        lastCharReceived = millis();

        _cmdBuffer[_cmdBufferPos] = nextChar;
        _cmdBufferPos++;
        if (_cmdBufferPos >= sizeof(_cmdBuffer)) {
            _logger->log("RS485 buffer overflow detected");
            _cmdBuffer[sizeof(_cmdBuffer)-1] = 0;
        }
    }
    digitalWrite(LED_BUILTIN, HIGH);

    if (_cmdBufferPos > 0 && millis() - lastCharReceived > TIMEOUT_MILLIS) {
        processCmdBuffer();
        _cmdBufferPos = 0;
    }
}

void RS485::processCmdBuffer() {
    battery.processFrame(_cmdBuffer, _cmdBufferPos);
    
}

void RS485::sendCommand(uint8_t* cmd, uint8_t size) {
    loop();
    beginTransmission();
    unsigned long start = micros();
    for (unsigned int i = 0; i < size; i++) {
        Serial.write(cmd[i]);
    }
    unsigned long duration = micros() - start;
    delayMicroseconds((size * 521) - duration + 200);
    duration = micros() - start;
    endTransmission();
    char buf[100];
    memcpy(buf, cmd, size);
    buf[size-1] = 0;
    _logger->log("Send: %s in %luns", buf, duration);
} 

void RS485::beginTransmission() {
    digitalWrite(_dePin, HIGH);
    delay(10);
    _transmitting = true;
}

void RS485::endTransmission() {
    // delayMicroseconds(_postDelay);
    digitalWrite(_dePin, LOW);
    _transmitting = false;
}
