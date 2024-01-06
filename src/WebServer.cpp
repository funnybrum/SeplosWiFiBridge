#include "Main.h"

char buffer[4096];

WebServer::WebServer(Logger* logger, NetworkSettings* networkSettings)
    :WebServerBase(networkSettings, logger) {
}

void WebServer::registerHandlers() {
    server->on("/", std::bind(&WebServer::handle_root, this));
    server->on("/settings", std::bind(&WebServer::handle_settings, this));
    server->on("/get", std::bind(&WebServer::handle_get, this));
    server->on("/heating/on", std::bind(&WebServer::handle_heat_on, this));
    server->on("/heating/off", std::bind(&WebServer::handle_heat_off, this));
    server->on("/cooling/on", std::bind(&WebServer::handle_cool_on, this));
    server->on("/cooling/off", std::bind(&WebServer::handle_cool_off, this));
    server->on("/balancer/on", std::bind(&WebServer::handle_balance_on, this));
    server->on("/balancer/off", std::bind(&WebServer::handle_balance_off, this));
    server->on("/balancer/auto", std::bind(&WebServer::handle_balance_auto, this));
}

void WebServer::handle_root() {
    server->sendHeader("Location","/settings");
    server->send(303);
}

void WebServer::handle_heat_on() {
    digitalWrite(D7, HIGH);
    server->send(200);
}

void WebServer::handle_heat_off() {
    digitalWrite(D7, LOW);
    server->send(200);
}

void WebServer::handle_cool_on() {
    digitalWrite(D6, HIGH);
    server->send(200);
}

void WebServer::handle_cool_off() {
    digitalWrite(D6, LOW);
    server->send(200);
}
void WebServer::handle_balance_on() {
    balancer.setMode(ON);
    server->send(200);
}

void WebServer::handle_balance_off() {
    balancer.setMode(OFF);
    server->send(200);
}

void WebServer::handle_balance_auto() {
    if (server->arg("threshold_voltage") != "") {
        int thresholdVoltage = atoi(server->arg("threshold_voltage").c_str());
        if (thresholdVoltage > 3650 || thresholdVoltage < 3300) {
            server->send(400, "Threshold voltage should be between 3300mV and 3650mV");
            return;
        }
        balancer.setThresholdVoltage(thresholdVoltage);
    }

    if (server->arg("threshold_current") != "") {
        int thresholdCurrent = atoi(server->arg("threshold_current").c_str());
        if (thresholdCurrent > 100 || thresholdCurrent < 0) {
            server->send(400, "Threshold current should be between 0A and 100A");
            return;
        }
        balancer.setThresholdCurrent(thresholdCurrent);
    }

    if (server->arg("voltage_diff") != "") {
        int thresholdVoltageDifference = atoi(server->arg("voltage_diff").c_str());
        if (thresholdVoltageDifference > 50 || thresholdVoltageDifference < 0) {
            server->send(400, "Threshold voltage difference should be between 0mV and 50mV");
            return;
        }
        balancer.setVoltageDifference(thresholdVoltageDifference);
    }

    balancer.setMode(AUTO);
    server->send(200);
}

void WebServer::handle_settings() {
    wifi.parse_config_params(this);
    dataCollector.parse_config_params(this);

    char network_settings[strlen_P(NETWORK_CONFIG_PAGE) + 32];
    wifi.get_config_page(network_settings);

    char influx_settings[strlen_P(INFLUXDB_CONFIG_PAGE) + 64];
    dataCollector.get_config_page(influx_settings);

    sprintf_P(
        buffer,
        CONFIG_PAGE,
        network_settings,
        influx_settings);
    server->send(200, "text/html", buffer);
}

//  "voltage":%.2f,
//  "port_voltage": %.2f,
//  "current":%.2f,
//  "cell_max": %.3f,
//  "cell_min": %.3f,
//  "soc": %.1f,
//  "heating": %d,
//  "comm_max_frame_delay": %d,
//  "rssi":%d


void WebServer::handle_get() {
    sprintf_P(buffer,
              GET_JSON,
              battery.getVoltage(),
              battery.getPortVoltage(),
              battery.getCurrent(),
              battery.getMaxCellVoltage(),
              battery.getMinCellVoltage(),
              battery.getSOC(),
              heating.isHeating(),
              battery.getMaxFrameDelay(),
              balancer.isBalancing(),
              WiFi.RSSI());
    server->send(200, "application/json", buffer);
}