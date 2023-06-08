#include "Main.h"

#define HEATING_PIN D7

void Heating::begin() {
    pinMode(HEATING_PIN, OUTPUT);
    digitalWrite(HEATING_PIN, LOW);
    heatingEnabled = false;
}

void Heating::loop() {
    float temp = min(battery.getCellTemp(0), battery.getCellTemp(1));
    temp = min(temp, battery.getCellTemp(2));
    temp = min(temp, battery.getCellTemp(3));

    if (temp < 14) {
        heatingEnabled = true;
    }

    if (temp > 15) {
        heatingEnabled = false;
    }

    digitalWrite(HEATING_PIN, heatingEnabled?HIGH:LOW);
}

bool Heating::isHeating() {
    return heatingEnabled;
}
