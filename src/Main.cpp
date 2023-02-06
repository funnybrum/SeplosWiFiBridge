#include "Main.h"

Logger logger = Logger();
Settings settings = Settings();

WiFiManager wifi = WiFiManager(&logger, &settings.getSettings()->network);
SystemCheck systemCheck = SystemCheck(&logger);
WebServer webServer = WebServer(&logger, &settings.getSettings()->network);
// RS485 rs485 = RS485(&logger);

void setup()
{ 
    Serial.begin(74880);
    while (! Serial) {
        delay(1);
    }
    settings.begin();
    wifi.begin();
    webServer.begin();
    systemCheck.begin();

    wifi.connect();
}

void loop() {
    wifi.loop();
    webServer.loop();
    settings.loop();
    systemCheck.loop();

    delay(100);
}
