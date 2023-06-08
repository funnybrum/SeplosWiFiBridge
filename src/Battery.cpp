#include "Main.h"


void Battery::processFrame(uint8_t* frame, uint16_t size) {

    uint8_t result[100];
    bool extractionResult = extractPayload(frame, size, result);

    if (!extractionResult) {
        logFrame(frame, size, 0);
        return;
    }

    maxFrameDelay = max(maxFrameDelay, (uint32)(millis() - lastReceivedFrame));
    lastReceivedFrame = millis();

    // uint8_t cellCount = result[0];
    for (int i = 0; i < 16; i++) {
        cellVoltage[i] = to3pDecimal(result, 1+i*2);
    }
    
    // uint8_t temperatureCount = result[33];
    for (int i = 0; i < 4; i++) {
        cellTemp[i] = toTemp(result, 34 + i * 2);
    }

    envTemp = toTemp(result, 40);
    bmsTemp = toTemp(result, 42);
    current = to2pDecimal(result, 46);
    voltage = to2pDecimal(result, 48);
    portVoltage = to2pDecimal(result, 63);
    soc = to1pDecimal(result, 55);

    remainingCapacity = to2pDecimal(result, 50);

    maxBatteryVoltage = max(maxBatteryVoltage, voltage);
    maxPortVoltage = max(maxPortVoltage, portVoltage);
    float maxCell = 0; float minCell = 100;
    for (int i = 0; i < 16; i++) {
        maxCell = max(maxCell, cellVoltage[i]);
        minCell = min(minCell, cellVoltage[i]);
    }
    maxCellVoltage = max(maxCellVoltage, maxCell);
    minCellVoltage = min(minCellVoltage, minCell);
    maxCellDiffVoltage = max(maxCellDiffVoltage, maxCell - minCell);
}

float Battery::getCellVoltage(uint8_t index) {
    return cellVoltage[index];
}

float Battery::getCellTemp(uint8_t index) {
    return cellTemp[index];
}

float Battery::getEnvTemp() {
    return envTemp;
}

float Battery::getBMSTemp() {
    return bmsTemp;
}

float Battery::getCurrent() {
    return current;
}

float Battery::getVoltage() {
    return voltage;
}

float Battery::getPortVoltage() {
    return portVoltage;
}

float Battery::getPower() {
    return current * voltage;
}

float Battery::getSOC() {
    return soc;
}

float Battery::getRemainingCapacity() {
    return remainingCapacity;
}

void Battery::logFrame(uint8_t* frame, uint16_t size, uint8_t code) {
    String result = "";

    char buf[16];
    sprintf(buf, "[err=%d]:[", code);
    result += buf;

    for (int i = 0; i < size; i++) {
        sprintf(buf, "%02X", frame[i]);
        result += buf;
        if (i < size - 1) {
            result += ", ";
        }
    }
    result += "]";
    logger.log(result.c_str());
}

/**
 * Verify the frame, extract the frame data and convert it from ASCII HEX to values.
 */
bool Battery::extractPayload(uint8_t frame[], uint16_t size, uint8_t result[]) {
    if (frame[0] != '~' || frame[size-1] != '\r') {
        return false;
    }
    
    // TODO verify checksum
    
    uint8_t buf[3];
    buf[2] = 0;

    for (int i = 17; i < size-2; i+=2) {
        buf[0] = (uint8_t)(*(frame + i));
        buf[1] = (uint8_t)(*(frame + i + 1));
        result[i/2 - 8] = (uint8_t) strtol((char*)buf, NULL, 16);
    }
    
    return true;
}


int16_t Battery::toDecimal(uint8_t data[], uint8_t pos) {
    return (data[pos] << 8) + data[pos+1];
}

float Battery::to3pDecimal(uint8_t data[], uint8_t pos) {
    return toDecimal(data, pos) * 0.001f;
}

float Battery::to2pDecimal(uint8_t data[], uint8_t pos) {
    return toDecimal(data, pos) * 0.01f;
}

float Battery::to1pDecimal(uint8_t data[], uint8_t pos) {
    return toDecimal(data, pos) * 0.1f;
}


float Battery::toTemp(uint8_t data[], uint8_t pos) {
    return (toDecimal(data, pos) - 2731) * 0.1f;
}

void Battery::begin() {
    lastDataRequest = 0;
    resetMaxValues();
}

void Battery::loop() {
    uint32_t cur_millis = millis();
    if (cur_millis - lastDataRequest > 1000) {
        uint8_t cmd[20] = {
            0x7E, 0x32, 0x30, 0x30, 0x30, 
            0x34, 0x36, 0x34, 0x32, 0x45, 
            0x30, 0x30, 0x32, 0x30, 0x30, 
            0x46, 0x44, 0x33, 0x37, 0x0D};
        rs485.sendCommand(cmd, 20);
        if (cur_millis - lastDataRequest < 1500) {
            lastDataRequest += 1000;
        } else {
            lastDataRequest = cur_millis;
        }
    }
}

void Battery::resetMaxValues() {
    this->maxBatteryVoltage = 0;
    this->maxPortVoltage = 0;
    this->maxCellVoltage = 0;
    this->maxFrameDelay = 0;
    this->minCellVoltage = 100;
    this->maxCellDiffVoltage = 0;
}

float Battery::getMaxBatteryVoltage() {
    return this->maxBatteryVoltage;
}

float Battery::getMaxPortVoltage() {
    return this->maxPortVoltage;
}

float Battery::getMaxCellVoltage() {
    return this->maxCellVoltage;
}

float Battery::getMinCellVoltage() {
    return this->minCellVoltage;
}

float Battery::getMaxCellDiffVoltage() {
    return this->maxCellDiffVoltage;
}

uint32_t Battery::getMaxFrameDelay() {
    return this->maxFrameDelay;
}