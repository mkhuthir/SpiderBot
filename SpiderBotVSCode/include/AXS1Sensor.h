#ifndef AXS1_SENSOR_H
#define AXS1_SENSOR_H

#include <DynamixelWorkbench.h>

class AXS1Sensor {
public:
    AXS1Sensor(DynamixelWorkbench* workbench, uint8_t sensor_id);

    bool ping();

    bool ledOn();
    bool ledOff();
    bool setBuzzer(uint8_t note, uint8_t duration);

    int getIRLeft();
    int getIRCenter();
    int getIRRight();

    int getLightLeft();
    int getLightCenter();
    int getLightRight();

    int getSoundLevel();
    int getSoundCount();

    float getVoltage();
    float getTemperature();

private:
    DynamixelWorkbench* dxl;
    uint8_t id;

    bool readItem(const char* item_name, int32_t* data);
    bool writeItem(const char* item_name, int32_t data);
};

#endif // AXS1_SENSOR_H
