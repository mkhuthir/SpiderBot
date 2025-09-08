#include "Servo.h"
#include "Leg.h"
#include "LegPoses.h"
#include "Kinematics.h"
#include "Console.h"
#include "Debug.h"


// Default constructor for Leg class
Leg::Leg(){
  // Initialize leg IDs to zero
  index            = 0;       // Leg index (0-5)
  baseX            = 0.0;     // Base X position in reference to body center
  baseY            = 0.0;     // Base Y position in reference to body center
  baseZ            = 0.0;     // Base Z position in reference to body center
  baseR            = 0.0;     // Base rotation in reference to body center
  servoIDs[Coxa]   = 0;       // Coxa servo ID
  servoIDs[Femur]  = 0;       // Femur servo ID
  servoIDs[Tibia]  = 0;       // Tibia servo ID
  driver           = nullptr; // Driver pointer
  servo            = nullptr; // Servo pointer
  speed            = 0;       // Leg speed

}

// Initialize the leg servos
bool Leg::init( uint8_t index,
                uint8_t coxaID, uint8_t femurID, uint8_t tibiaID, 
                float baseX, float baseY, float baseZ, 
                float baseR, uint8_t speed,
                Driver* driver, Servo* servo) {

  this->index      = index;      // Set leg index
  this->baseX      = baseX;      // Set leg base X position
  this->baseY      = baseY;      // Set leg base Y position
  this->baseZ      = baseZ;      // Set leg base height
  this->baseR      = baseR;      // Set leg base rotation
  servoIDs[Coxa]   = coxaID;     // Set coxa ID
  servoIDs[Femur]  = femurID;    // Set femur ID
  servoIDs[Tibia]  = tibiaID;    // Set tibia ID
  this->driver     = driver;     // Set the driver pointer
  this->servo      = servo;      // Set the servo pointer
  this->speed      = speed;      // Set default speed

  if (!servo->init(servoIDs[Coxa] , speed, COXA_CW_LIMIT, COXA_CCW_LIMIT)) {   // Initialize coxa servo with velocity
    LOG_ERR("Failed to initialize coxa servo.");
    return false;
  }
  if (!servo->init(servoIDs[Femur], speed, FEMUR_CW_LIMIT, FEMUR_CCW_LIMIT)) {   // Initialize femur servo with velocity
    LOG_ERR("Failed to initialize femur servo.");
    return false;
  }
  if (!servo->init(servoIDs[Tibia], speed, TIBIA_CW_LIMIT, TIBIA_CCW_LIMIT)) {   // Initialize tibia servo with velocity
    LOG_ERR("Failed to initialize tibia servo.");
    return false;
  }

  LOG_INF("Leg initialized successfully. (Servo IDs: " + String(servoIDs[Coxa]) + ", " + String(servoIDs[Femur]) + ", " + String(servoIDs[Tibia]) + ")");
  return true;

}

// Update the leg state
bool Leg::update() {
  // Update each servo
  for (int i = 0; i < LEG_SERVOS; i++) {
    servo->update(servoIDs[i]);
  }
  return true;
}

// Move the leg to the specified positions
bool Leg::move(int32_t *positions) {
  const uint8_t num_positions   = 1;
  if(!driver->syncWrite(handler_index, servoIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to move leg.");
    return false;
  }
  return true;
}

// Check if any servo in the leg is currently moving
bool Leg::isMoving() {
  uint8_t moving = 0;
  for (int i = 0; i < LEG_SERVOS; ++i) {
    if (servo->isMoving(servoIDs[i])) {
      return true;  // If any servo is moving, return true
    }
  }
  return false;
}

// Move leg up
bool Leg::movePointUp() {
  return move(poseLegPointUp);
}

// Move leg down
bool Leg::movePointDown() {
  return move(poseLegPointDown);
}

// Move leg out
bool Leg::movePointOut() {
  return move(poseLegPointOut);
}

// Move leg to stand up position
bool Leg::moveStandUp() {
  return move(poseLegStandUp);
}

// Move leg to stand down position
bool Leg::moveStandDown() {
  return move(poseLegStandDown);
}

// Set the speed of the leg
bool Leg::setSpeed(uint16_t speed) {
  // TODO: use syncWrite for setting speed
  if (speed < 0) speed = 0;
  if (speed > 1023) speed = 1023;
  for (int i = 0; i < LEG_SERVOS; i++) {
    if (!servo->setGoalSpeed(servoIDs[i], speed)) {
      LOG_ERR("Failed to set speed for leg servo.");
      return false;
    }
  }
  this->speed = speed;
  return true;
}

// Get the current speed of the leg
uint16_t Leg::getSpeed() const {
  return speed;
}

// Get the leg base position
bool Leg::getBasePosition(float* base_x, float* base_y, float* base_z, float* base_r) {
  *base_x = baseX;
  *base_y = baseY;
  *base_z = baseZ;
  *base_r = baseR;
  return true;
}

// Set servo positions
bool Leg::setServoPositions(uint16_t coxa, uint16_t femur, uint16_t tibia) {
  int32_t positions[LEG_SERVOS] = { static_cast<int32_t>(coxa), static_cast<int32_t>(femur), static_cast<int32_t>(tibia) };
  const uint8_t num_positions = 1;
  if (!driver->syncWrite(handler_index, servoIDs, LEG_SERVOS, positions, num_positions)) {
    LOG_ERR("Failed to set servo positions via syncWrite.");
    return false;
  }
  return true;
}

// Get the current positions of the leg joints
bool Leg::getServoPositions(uint16_t* coxa, uint16_t* femur, uint16_t* tibia) {
  // TODO: use syncRead
  if (!servo->getPresentPosition(servoIDs[Coxa], coxa)) {
    LOG_ERR("Failed to get coxa position.");
    return false;
  }
  if (!servo->getPresentPosition(servoIDs[Femur], femur)) {
    LOG_ERR("Failed to get femur position.");
    return false;
  }
  if (!servo->getPresentPosition(servoIDs[Tibia], tibia)) {
    LOG_ERR("Failed to get tibia position.");
    return false;
  }
  return true;
}

//---------------------------------------------------------------------------------------------

// Set the leg tip local position
bool Leg::setTipLocalPosition(float tip_local_x, float tip_local_y, float tip_local_z) {
  uint16_t positions[LEG_SERVOS];
  if (!IK::getIKLocal(tip_local_x, tip_local_y, tip_local_z, baseR, positions)) {
    LOG_ERR("Failed to compute inverse kinematics.");
    return false;
  }
  return setServoPositions(positions[Coxa], positions[Femur], positions[Tibia]);
}

// Get the leg tip local position
bool Leg::getTipLocalPosition(float* tip_local_x, float* tip_local_y, float* tip_local_z) {
  uint16_t coxa = 0, femur = 0, tibia = 0;
  if (!getServoPositions(&coxa, &femur, &tibia)) {
    LOG_ERR("Failed to get servo positions.");
    return false;
  }
  return IK::getFKLocal(coxa, femur, tibia, baseR, tip_local_x, tip_local_y, tip_local_z);
}

// Set the leg tip global position
bool Leg::setTipGlobalPosition(float tip_global_x, float tip_global_y, float tip_global_z) {
  float tip_local_x, tip_local_y, tip_local_z;
  IK::global2Local(tip_global_x, tip_global_y, tip_global_z, baseX, baseY, baseZ, &tip_local_x, &tip_local_y, &tip_local_z);
  return setTipLocalPosition(tip_local_x, tip_local_y, tip_local_z);
}

// Get the leg tip global position
bool Leg::getTipGlobalPosition(float* tip_global_x, float* tip_global_y, float* tip_global_z) {
  float tip_local_x, tip_local_y, tip_local_z;
  if (!getTipLocalPosition(&tip_local_x, &tip_local_y, &tip_local_z)) {
    LOG_ERR("Failed to get local tip position.");
    return false;
  }
  IK::local2Global(tip_local_x, tip_local_y, tip_local_z, baseX, baseY, baseZ, tip_global_x, tip_global_y, tip_global_z);
  return true;
}

//-------------------------------------------------------------------------------------

// Print leg status
bool Leg::printStatus() {
  uint16_t coxaAngle = 0, femurAngle = 0, tibiaAngle = 0;
  float baseX = 0, baseY = 0, baseZ = 0; float baseR = 0;
  float tip_local_X = 0, tip_local_Y = 0, tip_local_Z = 0;

  if (!getServoPositions(&coxaAngle, &femurAngle, &tibiaAngle)) {
    LOG_ERR("Failed to get joint angles.");
    return false;
  }

  if (!getBasePosition(&baseX, &baseY, &baseZ, &baseR)) {
    LOG_ERR("Failed to get leg base position.");
    return false;
  }

  if (!getTipLocalPosition(&tip_local_X, &tip_local_Y, &tip_local_Z)) {
    LOG_ERR("Failed to get leg tip position.");
    return false;
  }

  PRINTLN("Leg: " + String((int)index));
  PRINTLN("Base         : X: " + String((float)baseX)
                   + "mm, Y: " + String((float)baseY) 
                   + "mm, Z: " + String((float)baseZ) 
                   + "mm, R: " + String((float)baseR) + " deg");
  PRINTLN("Servo IDs    : Coxa: " + String((int)servoIDs[0]) 
              + ", Femur: " + String((int)servoIDs[1]) 
              + ", Tibia: " + String((int)servoIDs[2]));
  PRINTLN("Servo Pos.   : Coxa: " + String((int)coxaAngle) 
              + ", Femur: " + String((int)femurAngle) 
              + ", Tibia: " + String((int)tibiaAngle));
  PRINTLN("Speed        : " + String((int)speed));
  PRINTLN("Tip Local on : X: " + String((float)tip_local_X) 
                + "mm, Y: " + String((float)tip_local_Y) 
                + "mm, Z: " + String((float)tip_local_Z) + "mm");
  return true;
}

// Process console commands for leg control
bool Leg::runConsoleCommands(const String& cmd, const String& args, int index) {
    if (cmd == "ls") {
        printStatus();
        return true;

    } else if (cmd == "lss") {
        int newSpeed = LEG_SPEED; // Default speed

        if (args.length() > 0) {
            int count = 0, arg1 = 0, arg2 = 0;
            count = sscanf(args.c_str(), "%d %d", &arg1, &arg2);
            switch(count) {
              case 1:                 // only leg index is mentioned
                newSpeed = LEG_SPEED;
                break;
              case 2:
                newSpeed = arg2;      // leg index and speed are mentioned
                break;
              default:
                LOG_ERR("Invalid parameters");
                break;
            }
        }
        setSpeed(newSpeed);
        speed = newSpeed;
        LOG_INF("Leg " + String(index) + " speed: " + String((int)speed));
        return true;

    } else if (cmd == "lpu") {
        movePointUp();
        LOG_INF("Leg " + String(index) + " point moving up");
        return true;

    } else if (cmd == "lpd") {
        movePointDown();
        LOG_INF("Leg " + String(index) + " point moving down");
        return true;

    } else if (cmd == "lpo") {
        movePointOut();
        LOG_INF("Leg " + String(index) + " point moving out");
        return true;

    } else if (cmd == "lsu") {
        moveStandUp();
        LOG_INF("Leg " + String(index) + " standing up");
        return true;

    } else if (cmd == "lsd") {
        moveStandDown();
        LOG_INF("Leg " + String(index) + " standing down");
        return true;

    } else if (cmd == "lssp") {
        uint16_t coxaPos = COXA_DEFAULT, femurPos = FEMUR_DEFAULT, tibiaPos = TIBIA_DEFAULT;
        if (args.length() > 0) {
              int count = 0, arg1 = 0, arg2 = 0, arg3 = 0, arg4 = 0;
              count = sscanf(args.c_str(), "%d %d %d %d", &arg1, &arg2, &arg3, &arg4);
              switch(count) {
                case 1:                 // only leg index is mentioned
                  coxaPos = COXA_DEFAULT, femurPos = FEMUR_DEFAULT, tibiaPos = TIBIA_DEFAULT;
                  break;
                case 2:
                  coxaPos = arg2, femurPos = FEMUR_DEFAULT, tibiaPos = TIBIA_DEFAULT;
                  break;
                case 3:
                  coxaPos = arg2, femurPos = arg3, tibiaPos = TIBIA_DEFAULT;
                  break;
                case 4:
                  coxaPos = arg2, femurPos = arg3, tibiaPos = arg4;
                  break;
                default:
                  LOG_ERR("Invalid parameters");
                  break;
              }
        }
        setServoPositions(coxaPos, femurPos, tibiaPos);
        LOG_INF("Leg " + String(index) + " servo positions set to: Coxa: " + String(coxaPos) + ", Femur: " + String(femurPos) + ", Tibia: " + String(tibiaPos));
        return true;

    } else if (cmd == "lgsp") {
        uint16_t coxaPos = 0, femurPos = 0, tibiaPos = 0;
        getServoPositions(&coxaPos, &femurPos, &tibiaPos);
        LOG_INF("Leg " + String(index) + " servo positions: Coxa: " + String(coxaPos) + ", Femur: " + String(femurPos) + ", Tibia: " + String(tibiaPos));
        return true;

    } else if (cmd == "lstpl") {
        if (args.length() > 0) {
            int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
            count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
            if (count == 4) {
                setTipLocalPosition(local_x, local_y, local_z);
                LOG_INF("Leg " + String(index) + " tip local position set to: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
            } else {
                LOG_ERR("Invalid parameters for lstlp. Usage: lstlp n x y z");
            }
        } else {
            LOG_ERR("Invalid parameters for lstlp. Usage: lstlp n x y z");
        }
        return true;

    } else if (cmd == "lgtpl") {
        float tip_local_x = 0, tip_local_y = 0, tip_local_z = 0;
        getTipLocalPosition(&tip_local_x, &tip_local_y, &tip_local_z);
        LOG_INF("Leg " + String(index) + " tip local position: X: " + String(tip_local_x) + ", Y: " + String(tip_local_y) + ", Z: " + String(tip_local_z));
        return true;

    } else if (cmd == "lstpg") {
        if (args.length() > 0) {
            int count = 0, i = 0; float global_x = 0, global_y = 0, global_z = 0;
            count = sscanf(args.c_str(), "%d %f %f %f", &i, &global_x, &global_y, &global_z);
            if (count == 4) {
                setTipGlobalPosition(global_x, global_y, global_z);
                LOG_INF("Leg " + String(index) + " tip global position set to: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
            } else {
                LOG_ERR("Invalid parameters for lstgp. Usage: lstgp n x y z");
            }
        } else {
            LOG_ERR("Invalid parameters for lstgp. Usage: lstgp n x y z");
        }
        return true;

    } else if (cmd == "lgtpg") {
        float tip_global_x = 0, tip_global_y = 0, tip_global_z = 0;
        getTipGlobalPosition(&tip_global_x, &tip_global_y, &tip_global_z);
        LOG_INF("Leg " + String(index) + " tip global position: X: " + String(tip_global_x) + ", Y: " + String(tip_global_y) + ", Z: " + String(tip_global_z));
        return true;

    } else if (cmd == "lgikl") {
        int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
        if (count == 4) {
            uint16_t positions[LEG_SERVOS];
            if (IK::getIKLocal(local_x, local_y, local_z, baseR, positions)) {
                LOG_INF("Leg " + String(index) + " IK Local Positions: Coxa: " + String(positions[Coxa]) + ", Femur: " + String(positions[Femur]) + ", Tibia: " + String(positions[Tibia]));
            } else {
                LOG_ERR("Failed to compute IK Local.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgikl. Usage: lgikl n x y z");
        }
        return true;

    } else if (cmd == "lgikg") {
        int count = 0, i = 0; float global_x = 0, global_y = 0, global_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &global_x, &global_y, &global_z);
        if (count == 4) {
            uint16_t positions[LEG_SERVOS];
            if (IK::getIKGlobal(global_x, global_y, global_z, baseX, baseY, baseZ, baseR, positions)) {
                LOG_INF("Leg " + String(index) + " IK Global Positions: Coxa: " + String(positions[Coxa]) + ", Femur: " + String(positions[Femur]) + ", Tibia: " + String(positions[Tibia]));
            } else {
                LOG_ERR("Failed to compute IK Global.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgikg. Usage: lgikg n x y z");
        }
        return true;

    } else if (cmd == "lgfkl") {
        int count = 0, i = 0; uint16_t coxa = 0, femur = 0, tibia = 0;
        float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %hu %hu %hu", &i, &coxa, &femur, &tibia);
        if (count == 4) {
            if (IK::getFKLocal(coxa, femur, tibia, baseR,&local_x, &local_y, &local_z)) {
                LOG_INF("Leg " + String(index) + " FK Local Position: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
            } else {
                LOG_ERR("Failed to compute FK Local.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgfkl. Usage: lgfkl n c f t");
        }
        return true;

    } else if (cmd == "lgfkg") {
        int count = 0, i = 0; uint16_t coxa = 0, femur = 0, tibia = 0;
        float global_x = 0, global_y = 0, global_z = 0;
        count = sscanf(args.c_str(), "%d %hu %hu %hu", &i, &coxa, &femur, &tibia);
        if (count == 4) {
            if (IK::getFKGlobal(coxa, femur, tibia, baseX, baseY, baseZ, baseR, &global_x, &global_y, &global_z)) {
                LOG_INF("Leg " + String(index) + " FK Global Position: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
            } else {
                LOG_ERR("Failed to compute FK Global.");
            }
        } else {
            LOG_ERR("Invalid parameters for lgfkg. Usage: lgfkg n c f t");
        }
        return true;
    } else if (cmd == "lltg") {
        int count = 0, i = 0; float local_x = 0, local_y = 0, local_z = 0;
        float global_x = 0, global_y = 0, global_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &local_x, &local_y, &local_z);
        if (count == 4) {
            IK::local2Global(local_x, local_y, local_z, baseX, baseY, baseZ, &global_x, &global_y, &global_z);
            LOG_INF("Leg " + String(index) + " Local to Global: X: " + String(global_x) + ", Y: " + String(global_y) + ", Z: " + String(global_z));
        } else {
            LOG_ERR("Invalid parameters for lltg. Usage: lltg n x y z");
        }
        return true;
        
    } else if (cmd == "lgtl") {
        int count = 0, i = 0; float global_x = 0, global_y = 0, global_z = 0;
        float local_x = 0, local_y = 0, local_z = 0;
        count = sscanf(args.c_str(), "%d %f %f %f", &i, &global_x, &global_y, &global_z);
        if (count == 4) {
            IK::global2Local(global_x, global_y, global_z, baseX, baseY, baseZ, &local_x, &local_y, &local_z);
            LOG_INF("Leg " + String(index) + " Global to Local: X: " + String(local_x) + ", Y: " + String(local_y) + ", Z: " + String(local_z));
        } else {
            LOG_ERR("Invalid parameters for lgtl. Usage: lgtl n x y z");
        }
        return true;

    } else if (cmd == "l?") {
        printConsoleHelp();
        return true;

    }
    return false;
}

// Print leg-specific help information
bool Leg::printConsoleHelp() {
    PRINTLN("Leg Commands:\n\r");
    PRINTLN("  ls  [n]            - Print leg status (default: 0)");
    PRINTLN("  lss [n] [speed]    - Set leg speed (default: 0, " + String((int)LEG_SPEED) + ")");
    PRINTLN("");
    PRINTLN("  lpu [n]            - Move leg point up (default: 0)");
    PRINTLN("  lpd [n]            - Move leg point down (default: 0)");
    PRINTLN("  lpo [n]            - Move leg point out (default: 0)");
    PRINTLN("");
    PRINTLN("  lsu [n]            - Move leg to stand up position (default: 0)");
    PRINTLN("  lsd [n]            - Move leg to stand down position (default: 0)");
    PRINTLN("");
    PRINTLN("  lssp [n][c][f][t]  - Set leg servo positions (default: 0, " + String(COXA_DEFAULT) + ", " + String(FEMUR_DEFAULT) + ", " + String(TIBIA_DEFAULT) + ")");
    PRINTLN("  lgsp [n]           - Get leg servo positions (default: 0)");
    PRINTLN("");
    PRINTLN("  lstpl n x y z      - Set leg tip local position");
    PRINTLN("  lgtpl [n]          - Get leg tip local position (default: 0)");
    PRINTLN("");
    PRINTLN("  lstpg n x y z      - Set leg tip global position");
    PRINTLN("  lgtpg [n]          - Get leg tip global position (default: 0)");
    PRINTLN("");
    PRINTLN("  lgikl n x y z      - Compute IK in local coords (relative to leg base)");
    PRINTLN("  lgikg n x y z      - Compute IK in global coords (relative to body center)");
    PRINTLN("");
    PRINTLN("  lgfkl n c f t      - Compute FK in local coords (relative to leg base)");
    PRINTLN("  lgfkg n c f t      - Compute FK in global coords (relative to body center)");
    PRINTLN("");
    PRINTLN("  lltg n x y z       - Compute global coordinates from local coordinates");
    PRINTLN("  lgtl n x y z       - Compute local coordinates from global coordinates");
    PRINTLN("");
    PRINTLN("  l?                 - Show this help");
    PRINTLN("");
    return true;
}


