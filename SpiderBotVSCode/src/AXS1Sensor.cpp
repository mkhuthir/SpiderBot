#include "AXS1Sensor.h"

AXS1Sensor::AXS1Sensor(DynamixelWorkbench* workbench, uint8_t sensor_id)
    : dxl(workbench), id(sensor_id) {}

bool AXS1Sensor::ping() {
    uint16_t model_number = 0;
    return dxl->ping(id, &model_number);
}

bool AXS1Sensor::ledOn() {
    return writeItem("LED", 1);
}

bool AXS1Sensor::ledOff() {
    return writeItem("LED", 0);
}

bool AXS1Sensor::setBuzzer(uint8_t note, uint8_t duration) {
    bool ok = writeItem("Buzzer_Index", note);
    ok &= writeItem("Buzzer_Time", duration);
    return ok;
}

int AXS1Sensor::getIRLeft() {
    int32_t val = 0;
    if (readItem("IR_Left", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getIRCenter() {
    int32_t val = 0;
    if (readItem("IR_Center", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getIRRight() {
    int32_t val = 0;
    if (readItem("IR_Right", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getLightLeft() {
    int32_t val = 0;
    if (readItem("Light_Left", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getLightCenter() {
    int32_t val = 0;
    if (readItem("Light_Center", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getLightRight() {
    int32_t val = 0;
    if (readItem("Light_Right", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getSoundLevel() {
    int32_t val = 0;
    if (readItem("Sound_Level", &val))
        return static_cast<int>(val);
    return -1;
}

int AXS1Sensor::getSoundCount() {
    int32_t val = 0;
    if (readItem("Sound_Count", &val))
        return static_cast<int>(val);
    return -1;
}

float AXS1Sensor::getVoltage() {
    int32_t val = 0;
    if (readItem("Present_Voltage", &val))
        return static_cast<float>(val) / 10.0f;
    return -1.0f;
}

float AXS1Sensor::getTemperature() {
    int32_t val = 0;
    if (readItem("Present_Temperature", &val))
        return static_cast<float>(val);
    return -1.0f;
}

bool AXS1Sensor::readItem(const char* item_name, int32_t* data) {
    return dxl->itemRead(id, item_name, data);
}

bool AXS1Sensor::writeItem(const char* item_name, int32_t data) {
    return dxl->itemWrite(id, item_name, data);
}
