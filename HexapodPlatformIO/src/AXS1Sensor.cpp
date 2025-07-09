#include "AXS1Sensor.h"

AXS1Sensor::AXS1Sensor(DynamixelWorkbench* workbench, uint8_t sensor_id) {
    dxl = workbench;
    id = sensor_id;
}

bool AXS1Sensor::ping() {
    uint16_t model_number = 0;
    return dxl->ping(id, &model_number);
}

int AXS1Sensor::readTemperature() {
    uint8_t temperature = 0;
    if (dxl->itemRead(id, "Present Temperature", &temperature)) {
        return (int)temperature;
    }
    return -1;
}

int AXS1Sensor::readLuminosity() {
    uint16_t luminosity = 0;
    if (dxl->itemRead(id, "Present Illumination", &luminosity)) {
        return (int)luminosity;
    }
    return -1;
}

int AXS1Sensor::readSoundLevel() {
    uint8_t sound = 0;
    if (dxl->itemRead(id, "Present Sound Level", &sound)) {
        return (int)sound;
    }
    return -1;
}

bool AXS1Sensor::ledOn() {
    return dxl->itemWrite(id, "LED", 1);
}

bool AXS1Sensor::ledOff() {
    return dxl->itemWrite(id, "LED", 0);
}
