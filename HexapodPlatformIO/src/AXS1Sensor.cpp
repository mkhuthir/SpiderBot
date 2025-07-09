#include "AXS1Sensor.h"

// Constructor: Initializes the AXS1Sensor with a pointer to the DynamixelWorkbench and the sensor's ID
AXS1Sensor::AXS1Sensor(DynamixelWorkbench* workbench, uint8_t sensor_id) {
    dxl = workbench;
    id = sensor_id;
}

// Ping the sensor to check if it is connected and responding
bool AXS1Sensor::ping() {
    uint16_t model_number = 0;
    return dxl->ping(id, &model_number);
}

// Read the current temperature from the sensor
int AXS1Sensor::readTemperature() {
    uint8_t temperature = 0;
    // "Present Temperature" is the control table item for temperature
    if (dxl->itemRead(id, "Present Temperature", &temperature)) {
        return (int)temperature;
    }
    return -1; // Return -1 if reading fails
}

// Read the current luminosity (light level) from the sensor
int AXS1Sensor::readLuminosity() {
    uint16_t luminosity = 0;
    // "Present Illumination" is the control table item for luminosity
    if (dxl->itemRead(id, "Present Illumination", &luminosity)) {
        return (int)luminosity;
    }
    return -1; // Return -1 if reading fails
}

// Read the current sound level from the sensor
int AXS1Sensor::readSoundLevel() {
    uint8_t sound = 0;
    // "Present Sound Level" is the control table item for sound
    if (dxl->itemRead(id, "Present Sound Level", &sound)) {
        return (int)sound;
    }
    return -1; // Return -1 if reading fails
}

// Turn the sensor's LED on
bool AXS1Sensor::ledOn() {
    return dxl->itemWrite(id, "LED", 1);
}

// Turn the sensor's LED off
bool AXS1Sensor::ledOff() {
    return dxl->itemWrite(id, "LED", 0);
}
