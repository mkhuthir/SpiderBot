#include "DynamixelController.h"

DynamixelController::DynamixelController(const char* device, uint32_t baud)
  : device_name(device), baudrate(baud) {}

bool DynamixelController::begin()
{
  return dxl.begin(device_name, baudrate);
}

bool DynamixelController::ping(uint8_t id)
{
  uint16_t model_number;
  return dxl.ping(id, &model_number);
}

bool DynamixelController::torqueOn(uint8_t id)
{
  return dxl.torqueOn(id);
}

bool DynamixelController::torqueOff(uint8_t id)
{
  return dxl.torqueOff(id);
}

bool DynamixelController::setGoalPosition(uint8_t id, uint32_t position)
{
  return dxl.writeControlTableItem("Goal_Position", id, position);
}

bool DynamixelController::setGoalVelocity(uint8_t id, uint32_t velocity)
{
  return dxl.writeControlTableItem("Profile_Velocity", id, velocity);
}

bool DynamixelController::readPresentPosition(uint8_t id, uint32_t &position)
{
  return dxl.readControlTableItem("Present_Position", id, &position);
}
