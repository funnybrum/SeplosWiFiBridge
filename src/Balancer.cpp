#include "Main.h"

void Balancer::begin() {
    this->mode = OFF;
    this->thresholdVoltage = 3450;
    this->thresholdVoltageDiff = 5;
    this->isOn = false;
}

void Balancer::loop() {
    if (this->mode == OFF) {
        digitalWrite(D5, LOW);
        this->isOn = false;
        return;
    }

    if (this->mode == ON) {
        digitalWrite(D5, HIGH);
        this->isOn = true;
        return;
    }

    int highestCellVoltage = 0;
    int lowestCellVoltage = 9999;
    for (int i = 0; i < 16; i++) {
        highestCellVoltage = max(highestCellVoltage, int(battery.getCellVoltage(i)*1000));
        lowestCellVoltage = min(lowestCellVoltage, int(battery.getCellVoltage(i)*1000));
    }

    if (highestCellVoltage - lowestCellVoltage > this->thresholdVoltageDiff && highestCellVoltage > this->thresholdVoltage) {
        digitalWrite(D5, HIGH);
        this->isOn = true;
    } else {
        digitalWrite(D5, LOW);
        this->isOn = false;
    }
}

void Balancer::setMode(BalancerMode mode) {
    this->mode = mode;
}

void Balancer::setThresholdVoltage(int mV) {
    logger.log("Setting AB threshold voltage diff to %d", mV);
    this->thresholdVoltage = mV;
}

void Balancer::setVoltageDifference(int mV) {
    logger.log("Setting AB threshold voltage to %d", mV);
    this->thresholdVoltageDiff = mV;
}

bool Balancer::isBalancing() {
    return this->isOn;
}