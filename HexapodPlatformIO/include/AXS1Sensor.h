#ifndef AXS1SENSOR_H
#define AXS1SENSOR_H

#include <DynamixelWorkbench.h>

class AXS1Sensor {
private:
    DynamixelWorkbench* dxl;
    uint8_t id;

public:
    AXS1Sensor(DynamixelWorkbench* workbench, uint8_t sensor_id = 100);

    bool ping();
    int readTemperature();
    int readLuminosity();
    int readSoundLevel();

    bool ledOn();
    bool ledOff();
};

#endif  // AXS1Sensor.h
