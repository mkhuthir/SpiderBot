#include "DynamixelController.h"
#include <vector>

DynamixelController::DynamixelController() {}

bool DynamixelController::begin(const char* device_name, uint32_t baudrate) {
    return dxl.init(device_name, baudrate);
}

bool DynamixelController::initializeServo(uint8_t id) {
    bool result = dxl.itemWrite(id, "Operating_Mode", 3); // 3 = Position
    return result && torqueOn(id);
}

bool DynamixelController::torqueOn(uint8_t id) {
    return dxl.torqueOn(id);
}

bool DynamixelController::torqueOff(uint8_t id) {
    return dxl.torqueOff(id);
}

bool DynamixelController::setGoalPosition(uint8_t id, uint32_t position) {
    return dxl.itemWrite(id, "Goal_Position", position);
}

bool DynamixelController::setGoalVelocity(uint8_t id, uint32_t velocity) {
    return dxl.itemWrite(id, "Profile_Velocity", velocity);
}

bool DynamixelController::readPresentPosition(uint8_t id, uint32_t &position) {
    int32_t pos = 0;
    bool result = dxl.itemRead(id, "Present_Position", &pos);
    position = static_cast<uint32_t>(pos);
    return result;
}

bool DynamixelController::syncWritePosition(const std::vector<uint8_t>& ids, const std::vector<uint32_t>& positions) {
    if (ids.size() != positions.size() || ids.empty()) return false;
    std::vector<int32_t> pos32(positions.begin(), positions.end());
    uint8_t index = dxl.getIndex(ids[0], "Goal_Position");
    return dxl.syncWrite(index, const_cast<uint8_t*>(ids.data()), ids.size(), pos32.data(), 1);
}

bool DynamixelController::bulkReadPositions(const std::vector<uint8_t>& ids, std::vector<uint32_t>& positions) {
    positions.clear();
    for (size_t i = 0; i < ids.size(); ++i) {
        int32_t pos = 0;
        if (!dxl.itemRead(ids[i], "Present_Position", &pos)) {
            positions.push_back(0);
            return false;
        }
        positions.push_back(static_cast<uint32_t>(pos));
    }
    return true;
}

DynamixelWorkbench* DynamixelController::getWorkbench() {
    return &dxl;
}