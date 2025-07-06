#ifndef TURRET_H
#define TURRET_H

#include "DynamixelController.h"
#include "Config.h"


class Turret {
  public:
    Turret(uint8_t panID, uint8_t tiltID, uint8_t sensorID, DynamixelController* controller);
    void initialize();
    void rotateTurret(float panAngle, float tiltAngle);
    void resetTurret();
    int readSensor();

  private:
    uint8_t pan, tilt, sensor;
    DynamixelController* dxlCtrl;
};

#endif
