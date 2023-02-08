#include "Main.h"

RS485::RS485(Logger* logger, uint8_t dePin, uint16_t preDelay, uint16_t postDelay) {
    _logger = logger;
    _dePin = dePin;
    _cmdBufferPos = 0;
    _preDelay = preDelay;
    _postDelay = postDelay;
}

void RS485::begin(uint32_t baudRate, SerialConfig mode) {
    Serial.begin(baudRate);
    pinMode(_dePin, OUTPUT);
    digitalWrite(_dePin, LOW);
    _transmitting = false;
    _logger->log("RS485 initialized");
}

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
    // Serial.flush() does not work. Delay until all data is flushed.
    // 521ns is the required time to push 1 byte with SERIAL_8N1.
    delayMicroseconds((size * 521) - duration);
    endTransmission();
} 

void RS485::beginTransmission() {
    digitalWrite(_dePin, HIGH);
    delay(10);
    _transmitting = true;
}

void RS485::endTransmission() {
    delayMicroseconds(_postDelay);
    digitalWrite(_dePin, LOW);
    _transmitting = false;
}
