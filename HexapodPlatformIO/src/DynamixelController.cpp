#include "DynamixelController.h"

DynamixelController::DynamixelController(HardwareSerial& serial, uint8_t dirPin)
  : dxl(serial, dirPin) {}

void DynamixelController::begin(int baudrate) {
  dxl.begin(baudrate);
  dxl.setPortProtocolVersion(2.0);
}

void DynamixelController::initializeServo(uint8_t id, float minAngle, float maxAngle) {
  dxl.torqueOff(id);
  dxl.setOperatingMode(id, OP_POSITION);
  dxl.torqueOn(id);
  limits[id] = {minAngle, maxAngle};
}

void DynamixelController::setPosition(uint8_t id, float angleDeg) {
  if (angleDeg < limits[id].minAngle) angleDeg = limits[id].minAngle;
  if (angleDeg > limits[id].maxAngle) angleDeg = limits[id].maxAngle;
  dxl.setGoalPosition(id, angleDeg, UNIT_DEGREE);
}

void DynamixelController::setSpeed(uint8_t id, float rpm) {
  dxl.setGoalVelocity(id, rpm, UNIT_RPM);
}

float DynamixelController::readPosition(uint8_t id) {
  return dxl.getPresentPosition(id, UNIT_DEGREE);
}
