#include "Main.h"

Logger logger = Logger(false);
Settings settings = Settings();

WiFiManager wifi = WiFiManager(&logger, &settings.getSettings()->network);
WebServer webServer = WebServer(&logger, &settings.getSettings()->network);
DataCollector dataCollector = DataCollector();
RS485 rs485 = RS485(&logger);
Battery battery = Battery();

void setup()
{
    Serial.begin(19200);
    delay(100);
    settings.begin();
    wifi.begin();
    webServer.begin();

    wifi.connect();
    dataCollector.begin();
    battery.begin();
    rs485.begin();
    pinMode(LED_BUILTIN, OUTPUT);
}

int i = 0;

void loop() {
    wifi.loop();
    webServer.loop();
    settings.loop();
    dataCollector.loop();
    battery.loop();
    rs485.loop();

    if (wifi.isInAPMode()) {
        if (millis() > 15 * 1000) {
            ESP.reset();
        }
        digitalWrite(LED_BUILTIN, i%30);
    } else if (wifi.isConnected()) {
        digitalWrite(LED_BUILTIN, i%10);
    } else {
        if (millis() > 15 * 1000) {
            ESP.reset();
        }
        digitalWrite(LED_BUILTIN, i%2);
    }

    i++;

    delay(100);
}
