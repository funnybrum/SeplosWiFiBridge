#pragma once

#include "Arduino.h"

class Heating {
    public:
        void begin();
        void loop();
        bool isHeating();

    private:
        uint8_t heatingEnabled;
};
