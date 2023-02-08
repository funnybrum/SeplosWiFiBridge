#include "Main.h"

char buffer[4096];

WebServer::WebServer(Logger* logger, NetworkSettings* networkSettings)
    :WebServerBase(networkSettings, logger) {
}

void WebServer::registerHandlers() {
    server->on("/", std::bind(&WebServer::handle_root, this));
    server->on("/settings", std::bind(&WebServer::handle_settings, this));
    server->on("/get", std::bind(&WebServer::handle_get, this));
    server->on("/nl0", std::bind(&WebServer::handle_nl0, this));
    server->on("/nl1", std::bind(&WebServer::handle_nl1, this));
}

void WebServer::handle_root() {
    server->sendHeader("Location","/settings");
    server->send(303);
}

void WebServer::handle_nl0() {
    uint8_t cmd[20] = {
        0x7E, 0x32, 0x30, 0x30, 0x30, 
        0x34, 0x36, 0x34, 0x32, 0x45, 
        0x30, 0x30, 0x32, 0x30, 0x30, 
        0x46, 0x44, 0x33, 0x37, 0x0D};
    rs485.sendCommand(cmd, 20);
    server->send(200);
}

void WebServer::handle_nl1() {
    uint8_t cmd[20] = {
        0x7E, 0x32, 0x30, 0x30, 0x31, 
        0x34, 0x36, 0x34, 0x32, 0x45, 
        0x30, 0x30, 0x32, 0x30, 0x31, 
        0x46, 0x44, 0x33, 0x35, 0x0D};
    rs485.sendCommand(cmd, 20);
    server->send(200);
}

void WebServer::handle_settings() {
    wifi.parse_config_params(this);

    char network_settings[strlen_P(NETWORK_CONFIG_PAGE) + 32];
    wifi.get_config_page(network_settings);


    sprintf_P(
        buffer,
        CONFIG_PAGE,
        network_settings);
    server->send(200, "text/html", buffer);
}

void WebServer::handle_get() {
    sprintf_P(buffer,
              GET_JSON,
              battery.getVoltage(),
              battery.getCurrent(),
              battery.getVoltage() * battery.getCurrent(),
              battery.getCellVoltage(0),
              battery.getCellVoltage(1),
              battery.getCellVoltage(2),
              battery.getCellVoltage(3),
              battery.getCellVoltage(4),
              battery.getCellVoltage(5),
              battery.getCellVoltage(6),
              battery.getCellVoltage(7),
              battery.getCellVoltage(8),
              battery.getCellVoltage(9),
              battery.getCellVoltage(10),
              battery.getCellVoltage(11),
              battery.getCellVoltage(12),
              battery.getCellVoltage(13),
              battery.getCellVoltage(14),
              battery.getCellVoltage(15),
              battery.getCellTemp(0),
              battery.getCellTemp(1),
              battery.getCellTemp(2),
              battery.getCellTemp(3),
              battery.getEnvTemp(),
              battery.getBMSTemp(),
              battery.getSOC(),
              WiFi.RSSI());
    server->send(200, "application/json", buffer);
}