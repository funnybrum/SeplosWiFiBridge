#include "Main.h"
#include "DataCollector.h"

DataCollector::DataCollector():
    InfluxDBCollector(&logger,
                      NULL,
                      &settings.getSettings()->influxDB,
                      &settings.getSettings()->network) {
}

bool DataCollector::shouldCollect() {
    return millis() > 20000;
}

void DataCollector::collectData() {
    append("battery.voltage.cell1", battery.getCellVoltage(0), 3);
    append("battery.voltage.cell2", battery.getCellVoltage(1), 3);
    append("battery.voltage.cell3", battery.getCellVoltage(2), 3);
    append("battery.voltage.cell4", battery.getCellVoltage(3), 3);
    append("battery.voltage.cell5", battery.getCellVoltage(4), 3);
    append("battery.voltage.cell6", battery.getCellVoltage(5), 3);
    append("battery.voltage.cell7", battery.getCellVoltage(6), 3);
    append("battery.voltage.cell8", battery.getCellVoltage(7), 3);
    append("battery.voltage.cell9", battery.getCellVoltage(8), 3);
    append("battery.voltage.cell10", battery.getCellVoltage(9), 3);
    append("battery.voltage.cell11", battery.getCellVoltage(10), 3);
    append("battery.voltage.cell12", battery.getCellVoltage(11), 3);
    append("battery.voltage.cell13", battery.getCellVoltage(12), 3);
    append("battery.voltage.cell14", battery.getCellVoltage(13), 3);
    append("battery.voltage.cell15", battery.getCellVoltage(14), 3);
    append("battery.voltage.cell16", battery.getCellVoltage(15), 3);
    append("battery.voltage.battery", battery.getVoltage(), 3);
    append("battery.temp.cell_sensor1", battery.getCellTemp(0), 1);
    append("battery.temp.cell_sensor2", battery.getCellTemp(1), 1);
    append("battery.temp.cell_sensor3", battery.getCellTemp(2), 1);
    append("battery.temp.cell_sensor4", battery.getCellTemp(3), 1);
    append("battery.temp.env", battery.getEnvTemp(), 1);
    append("battery.temp.bms", battery.getBMSTemp(), 1);
    append("battery.energy.power", battery.getPower(), 0);
    append("battery.energy.soc", battery.getSOC(), 1);
    append("battery.energy.remaining_ah", battery.getRemainingCapacity(), 2);
}

bool DataCollector::shouldPush() {
    return false;
}   

void DataCollector::beforePush() {
    append("rssi", WiFi.RSSI(), 0);
}

void DataCollector::afterPush() {
}