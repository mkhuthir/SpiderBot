#ifdef AXS1SENSOR_H
#define AXS1SENSOR_H

#include <DynamixelWorkbench.h>
#include "Config.h"

// AXS1Sensor class provides an interface to the AXS-1 sensor module
class AXS1Sensor {
private:
    DynamixelWorkbench* dxl;    // Pointer to the DynamixelWorkbench for communication
    uint8_t             id;     // ID of the AXS-1 sensor

public:
    // Constructor: Initializes the sensor with a DynamixelWorkbench pointer and optional sensor ID (default 100)
    AXS1Sensor(DynamixelWorkbench* workbench, uint8_t sensor_id = AX_S1_SENSOR_ID);

   
    bool    ping();             // Ping the sensor to check if it is connected and responding
    int     readTemperature();  // Read the current temperature from the sensor
    int     readLuminosity();   // Read the current luminosity (light level) from the sensor
    int     readSoundLevel();   // Read the current sound level from the sensor
    bool    ledOn();            // Turn the sensor's LED on
    bool    ledOff();           // Turn the sensor's LED off
};

#endif  // AXS1SENSOR_H
