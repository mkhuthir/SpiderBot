#ifndef CONFIG_H
#define CONFIG_H

// ------------------------
// SERIAL PORTS
// ------------------------

// When using DynamixelShield
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
  #include <SoftwareSerial.h>
  SoftwareSerial soft_serial(7, 8);
  #define DXL_SERIAL   Serial
  #define RC_SERIAL    Serial2
  #define DEBUG_SERIAL soft_serial
  const int DXL_DIR_PIN = 2;

// When using DynamixelShield
#elif defined(ARDUINO_SAM_DUE)
  #define DXL_SERIAL   Serial
  #define RC_SERIAL    Serial2
  #define DEBUG_SERIAL SerialUSB
  const int DXL_DIR_PIN = 2;

// When using DynamixelShield
#elif defined(ARDUINO_SAM_ZERO)
  #define DXL_SERIAL   Serial1
  #define RC_SERIAL    Serial2
  #define DEBUG_SERIAL SerialUSB
  const int DXL_DIR_PIN = 2;

// When using official ROBOTIS board with DXL circuit.
#elif defined(ARDUINO_OpenCM904)
  #define DXL_SERIAL   Serial3
  #define RC_SERIAL    Serial1
  #define DEBUG_SERIAL Serial
  const int DXL_DIR_PIN = 22; //OpenCM9.04 EXP Board's DIR PIN. (28 for the DXL port on the OpenCM 9.04 board)

// When using official ROBOTIS OpenCR board with DXL circuit.
#elif defined(ARDUINO_OpenCR)
  #define DEBUG_SERIAL Serial
  #define RC_SERIAL    Serial1
  #define DXL_SERIAL   Serial3
  const int DXL_DIR_PIN = 84;

// When using OpenRB-150
#elif defined(ARDUINO_OpenRB)
  #define DXL_SERIAL Serial1
  #define RC_SERIAL    Serial2
  #define DEBUG_SERIAL Serial
  const int DXL_DIR_PIN = -1;  //OpenRB does not require the DIR control pin.

// Other boards when using DynamixelShield
#else
  #define DXL_SERIAL   Serial1
  #define RC_SERIAL    Serial2
  #define DEBUG_SERIAL Serial
  const int DXL_DIR_PIN = 2;

#endif

// ------------------------
// SERIAL BAUD RATES
// ------------------------

#define DXL_BAUD_RATE         1000000   // Default baud rate for Dynamixel servos
#define DEBUG_BAUD_RATE       115200    // Baud rate for debugging output
#define RC_BAUD_RATE          115200    // Baud rate for RC controller communication

// ------------------------
// DYNAMIXEL PROTOCOL VERSION
// ------------------------

#define DXL_PROTOCOL_VERSION 1.0        // Default protocol version for Dynamixel servos

// ------------------------
// SERVO IDs
// ------------------------

// Legs (IDs 1 to 18)
#define LEG_SERVO_ID_START    1
#define LEG_SERVO_ID_END      18

// Turret servos
#define TURRET_PAN_ID         19
#define TURRET_TILT_ID        20

// AX-S1 Sensor
#define AX_S1_SENSOR_ID       100

// ------------------------
// SERVO LIMITS
// Define safe limits to avoid mechanical overrun
// AX-18A typical physical limit ~0-300 deg

#define SERVO_MIN_ANGLE_DEG         0   // Min angle for AX-18A servos
#define SERVO_MAX_ANGLE_DEG         300 // Max angle for AX-18A servos
#define MAX_NO_OF_SERVOS            256 // Max number of servos supported by Dynamixel2Arduino

#define TURRET_PAN_MIN_ANGLE_DEG    0   // Min angle for turret pan servo
#define TURRET_PAN_MAX_ANGLE_DEG    300 // Max angle for turret pan servo
#define TURRET_PAN_DEFAULT_DEG      150 // Default pan angle for turret

#define TURRET_TILT_MIN_ANGLE_DEG   0   // Min angle for turret tilt servo
#define TURRET_TILT_MAX_ANGLE_DEG   300 // Max angle for turret tilt servo
#define TURRET_TILT_DEFAULT_DEG     60  // Default tilt angle for turret    



// ------------------------
// GAIT TIMING
// ------------------------
#define DEFAULT_GAIT_CYCLE_MS 1000

#endif
