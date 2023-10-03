#pragma once

#include "Main.h"
#include "WebServerBase.h"
#include "WebPages.h"

class WebServer: public WebServerBase {
    public:
        WebServer(Logger* logger, NetworkSettings* networkSettings);
        void registerHandlers();
        void handle_root();
        void handle_get();
        void handle_settings();
        void handle_reset();
        void handle_blink();
        void handle_heat_on();
        void handle_heat_off();
        void handle_cool_on();
        void handle_cool_off();
        void handle_balance_on();
        void handle_balance_off();
        void handle_balance_auto();
};
