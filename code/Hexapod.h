#ifndef HEXAPOD_H
#define HEXAPOD_H

  #include "Leg.h"
  #include "Driver.h"

  // Hexapod configuration constants
  #define handler_index  uint8_t(0)                                         // Index for sync write handler
  #define HEXAPOD_LEGS   uint8_t(6)                                         // Maximum number of legs
  #define LEG_SERVOS     uint8_t(3)                                         // Number of servos per leg
  #define HEXAPOD_SERVOS uint8_t(HEXAPOD_LEGS * LEG_SERVOS)                 // Maximum number of servos
  #define HEXAPOD_SPEED  uint16_t(100)                                      // Default speed for hexapod

  #define LEG_0_BASE_X   float(120.0)                                       // X position for leg 0 in mm from body center
  #define LEG_0_BASE_Y   float(-60.0)                                       // Y position for leg 0 in mm from body center
  #define LEG_0_BASE_Z   float(0.0)                                         // Z position for leg 0 in mm from body center
  #define LEG_0_BASE_R   float(-45.0)                                      // Rotation for leg 0 in radians

  #define LEG_1_BASE_X   float(120.0)                                       // X position for leg 1 in mm from body center
  #define LEG_1_BASE_Y   float(60.0)                                        // Y position for leg 1 in mm from body center
  #define LEG_1_BASE_Z   float(0.0)                                         // Z position for leg 1 in mm from body center
  #define LEG_1_BASE_R   float(45.0)                                       // Rotation for leg 1 in radians

  #define LEG_2_BASE_X   float(0.0)                                         // X position for leg 2 in mm from body center
  #define LEG_2_BASE_Y   float(-60.0)                                       // Y position for leg 2 in mm from body center
  #define LEG_2_BASE_Z   float(0.0)                                         // Z position for leg 2 in mm from body center
  #define LEG_2_BASE_R   float(-90.0)                                       // Rotation for leg 2 in radians

  #define LEG_3_BASE_X   float(0.0)                                         // X position for leg 3 in mm from body center
  #define LEG_3_BASE_Y   float(60.0)                                        // Y position for leg 3 in mm from body center
  #define LEG_3_BASE_Z   float(0.0)                                         // Z position for leg 3 in mm from body center
  #define LEG_3_BASE_R   float(90.0)                                       // Rotation for leg 3 in radians

  #define LEG_4_BASE_X   float(-120.0)                                      // X position for leg 4 in mm from body center
  #define LEG_4_BASE_Y   float(-60.0)                                       // Y position for leg 4 in mm from body center
  #define LEG_4_BASE_Z   float(0.0)                                         // Z position for leg 4 in mm from body center
  #define LEG_4_BASE_R   float(-135.0)                                       // Rotation for leg 4 in radians

  #define LEG_5_BASE_X   float(-120.0)                                      // X position for leg 5 in mm from body center
  #define LEG_5_BASE_Y   float(60.0)                                        // Y position for leg 5 in mm from body center
  #define LEG_5_BASE_Z   float(0.0)                                         // Z position for leg 5 in mm from body center
  #define LEG_5_BASE_R   float(135.0)                                      // Rotation for leg 5 in radians


  class Hexapod {
    public:
      Hexapod(); // Constructor
      bool      begin(Driver* driver, Servo* servo);                        // Initialize the hexapod
      bool      update();                                                   // Update the hexapod state

      bool      move(uint8_t *ids, uint8_t num_servos, int32_t *positions); // Move Hexapod
      bool      isMoving();                                                 // Check if any leg is currently moving
      bool      moveStandUp();                                              // Move Hexapod Up
      bool      moveStandDown();                                            // Move Hexapod Down

      bool      setSpeed(uint16_t speed);                                   // Set the speed of the hexapod
      uint16_t  getSpeed() const;                                           // Get the current speed of the hexapod

      bool      printStatus();                                              // Print the status of all legs
      bool      runConsoleCommands(const String& cmd, const String& args);  // Process console commands for hexapod control
      bool      printConsoleHelp();                                         // Print hexapod-specific help information
      
      Leg     legs[HEXAPOD_LEGS];                                           // Array of legs

    private:
      Driver*  driver;                                                      // Dynamixel controller instance
      Servo*   servo;                                                       // Servo controller instance
      uint16_t speed;                                                       // Current speed of the hexapod


  };

#endif // HEXAPOD_H