#include "DynamixelController.h"

DynamixelController::DynamixelController(const char* device, uint32_t baud)
  : device_name(device), baudrate(baud) {}

bool DynamixelController::begin()
{
  return dxl.begin(device_name, baudrate);
}

void DynamixelController::initializeServo(uint8_t id) {
    dxl.torqueOff(id);
    dxl.setOperatingMode(id, OP_POSITION); // or OP_VELOCITY / OP_PWM if you want
    dxl.torqueOn(id);
}

void DynamixelController::moveServo(uint8_t id, uint16_t position, uint16_t speed) {
    dxl.goalVelocity(id, speed);
    dxl.goalPosition(id, position);
}

void DynamixelController::enableTorque(uint8_t id) {
    dxl.torqueOn(id);
}

void DynamixelController::disableTorque(uint8_t id) {
    dxl.torqueOff(id);
}

bool DynamixelController::ping(uint8_t id)
{
  uint16_t model_number;
  return dxl.ping(id, &model_number);
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

std::vector<uint8_t> DynamixelController::scan(uint8_t start_id, uint8_t end_id)
{
  std::vector<uint8_t> found_ids;
  for (uint8_t id = start_id; id <= end_id; ++id)
  {
    uint16_t model_number;
    if (dxl.ping(id, &model_number))
    {
      found_ids.push_back(id);
    }
  }
  return found_ids;
}

// Sync Write to multiple servos
bool DynamixelController::syncWritePosition(const std::vector<uint8_t>& ids, const std::vector<uint32_t>& positions)
{
  if (ids.size() != positions.size()) return false;
  if (ids.empty()) return false;

  const char* item_name = "Goal_Position";
  const uint16_t addr = dxl.getControlTableItemAddress(item_name, ids[0]);
  const uint16_t length = dxl.getControlTableItemSize(item_name, ids[0]);

  if (addr == 0 || length == 0) return false;

  uint8_t param[ids.size() * length];
  for (size_t i = 0; i < ids.size(); ++i)
  {
    for (uint16_t b = 0; b < length; ++b)
    {
      param[i * length + b] = (positions[i] >> (8 * b)) & 0xFF;
    }
  }

  return dxl.syncWrite(addr, length, ids.data(), ids.size(), param, ids.size() * length);
}

// Bulk Read Present_Position
bool DynamixelController::bulkReadPositions(const std::vector<uint8_t>& ids, std::vector<uint32_t>& positions)
{
  positions.clear();
  if (ids.empty()) return false;

  positions.reserve(ids.size());
  for (uint8_t id : ids)
  {
    uint32_t pos;
    if (dxl.readControlTableItem("Present_Position", id, &pos))
    {
      positions.push_back(pos);
    }
    else
    {
      positions.push_back(0xFFFFFFFF); // Error value
    }
  }
  return true;
}
