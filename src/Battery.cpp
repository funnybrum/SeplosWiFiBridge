#include "Main.h"


float toFloat(uint8_t* p) {
    return toDecimal(p) * 0.01f;
}

float toTemp(uint8_t* p) {
    return (toDecimal(p) - 2713) * 0.1f;
}

int16_t toDecimal(uint8_t* p) {
    return  ((p[0]-30) << 12) + ((p[1]-30) << 8) + ((p[3]-30) << 4) + (p[4]-30);
}


void Battery::processFrame(uint8_t* frame, uint16_t size) {
    if (size != 168) {
        // Unexpected frame size
        logFrame(frame, size, 0);
        return;        
    }

    if (frame[0] != '~') {
        // Unknown frame
        logFrame(frame, size, 1);
        return;
    }
    
    if (frame[7] != 0 || frame[8] != 0) {
        // Error
        logFrame(frame, size, 2);
        return;
    }

    if (frame[11] != 9 || frame[12] != 6) {
        // Frame with unknown length
        logFrame(frame, size, 3);
        return;
    }

    if (frame[17] != 1 || frame[18] != 0) {
        // Unexpected cell count
        logFrame(frame, size, 4);
        return;
    }

    if (frame[83] != 0 || frame[84] != 6) {
        // Unexpected temperature sensors count
        logFrame(frame, size, 5);
        return;
    }

    if (frame[167] != 0x0D) {
        // Unexpected frame ending character
        logFrame(frame, size, 6);
        return;
    }

    for (int i = 0; i < 16; i++) {
        cellVoltage[i] = toDecimal(frame + 19 + (4*i));
    }

    for (int i = 0; i < 4; i++) {
        cellTemp[i] = toTemp(frame + 85 + (4*i));
    }

    envTemp = 

}

float* Battery::getCellVoltages() {
    return cellVoltage;
}

float* Battery::getCellTemp() {
    return cellTemp;
}

float Battery::getEnvTemp() {
    return envTemp;
}

float Battery::getBMSTemp() {
    return bmsTemp;
}

float Battery::getCurrent() {
    return current;
}

float Battery::getVoltage() {
    return voltage;
}

float Battery::getPower() {
    return current * voltage;
}

void Battery::logFrame(uint8_t* frame, uint16_t size, uint8_t code) {
    String result = "";

    char buf[16];
    sprintf(buf, "[err=%d]:[", code);

    for (int i = 0; i < size; i++) {
        sprintf(buf, "%02X", frame[i]);
        result += buf;
        if (i < size - 1) {
            result += ", ";
        }
    }
    result += "]";
    logger.log(result.c_str());
}
