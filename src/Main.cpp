#include "Main.h"

Logger logger = Logger(false);
Settings settings = Settings();

WiFiManager wifi = WiFiManager(&logger, &settings.getSettings()->network);
WebServer webServer = WebServer(&logger, &settings.getSettings()->network);
DataCollector dataCollector = DataCollector();
RS485 rs485 = RS485(&logger);
Battery battery = Battery();
Heating heating = Heating();
Balancer balancer = Balancer();

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
    heating.begin();
    balancer.begin();

    pinMode(LED_BUILTIN, OUTPUT);

    // balancing out
    pinMode(D5, OUTPUT);
    digitalWrite(D5, LOW);

    // 48v out 1
    pinMode(D6, OUTPUT);
    digitalWrite(D6, LOW);

    // 48v out 2
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);
}

int i = 0;

void loop() {
    wifi.loop();
    webServer.loop();
    settings.loop();
    dataCollector.loop();
    battery.loop();
    rs485.loop();
    heating.loop();
    balancer.loop();

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
