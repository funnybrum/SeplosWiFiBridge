#include "Main.h"

void Balancer::begin() {
    this->mode = AUTO;
    this->thresholdVoltage = 3440;
    this->thresholdVoltageDiff = 5;
    this->thresholdCurrent = 10;
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

    if (highestCellVoltage > this->thresholdVoltage &&
        abs(battery.getCurrent()) < this->thresholdCurrent) {
        
        int vDiff = highestCellVoltage - lowestCellVoltage;
        if (this->isOn) {
            // Add 2mV as hysteresis.
            vDiff += 2;
        }
        if (vDiff > this->thresholdVoltageDiff) {
            digitalWrite(D5, HIGH);
            this->isOn = true;
        } else {
            digitalWrite(D5, LOW);
            this->isOn = false;
        }
    } else {
        digitalWrite(D5, LOW);
        this->isOn = false;
    }
}

void Balancer::setMode(BalancerMode mode) {
    this->mode = mode;
}

void Balancer::setThresholdVoltage(int mV) {
    logger.log("Setting AB threshold voltage to %d", mV);
    this->thresholdVoltage = mV;
}

void Balancer::setVoltageDifference(int mV) {
    logger.log("Setting AB threshold voltage to %d", mV);
    this->thresholdVoltageDiff = mV;
}

void Balancer::setThresholdCurrent(int a) {
    logger.log("Setting AB threshold current to %dA", a);
    this->thresholdCurrent = a;
}

bool Balancer::isBalancing() {
    return this->isOn;
}