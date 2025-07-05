#include "Turret.h"

Turret::Turret(uint8_t panID, uint8_t tiltID, uint8_t sensorID, DynamixelController* controller)
  : pan(panID), tilt(tiltID), sensor(sensorID), dxlCtrl(controller) {}

void Turret::initialize() {
  dxlCtrl->initializeServo(pan, -90, 90);
  dxlCtrl->initializeServo(tilt, -45, 45);
}

void Turret::rotateTurret(float panAngle, float tiltAngle) {
  dxlCtrl->setPosition(pan, panAngle);
  dxlCtrl->setPosition(tilt, tiltAngle);
}

void Turret::resetTurret() {
  rotateTurret(0,0);
}

int Turret::readSensor() {
  return dxlCtrl->readPosition(sensor);
}
