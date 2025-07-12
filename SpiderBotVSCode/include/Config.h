#ifndef CONFIG_H
#define CONFIG_H

// Config.h - Configuration file for SpiderBot project

// ------------------------
// SERIAL PORTS
// ------------------------

#define DEBUG_SERIAL Serial
#define RC100_SERIAL Serial1
#define DXL_SERIAL   "1"


// ------------------------
// SERIAL BAUD RATES
// ------------------------

#define DXL_BAUD_RATE         1000000   // Default baud rate for Dynamixel servos
#define DEBUG_BAUD_RATE       115200    // Baud rate for debugging output
#define RC10_BAUD_RATE        115200    // Baud rate for RC controller communication

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

#define MAX_NO_OF_SERVOS            256 // Max number of servos supported by Dynamixel2Arduino

#define TURRET_PAN_MIN_ANGLE_DEG    0   // Min angle for turret pan servo
#define TURRET_PAN_MAX_ANGLE_DEG    300 // Max angle for turret pan servo
#define TURRET_PAN_DEFAULT_DEG      150 // Default pan angle for turret

#define TURRET_TILT_MIN_ANGLE_DEG   0   // Min angle for turret tilt servo
#define TURRET_TILT_MAX_ANGLE_DEG   300 // Max angle for turret tilt servo
#define TURRET_TILT_DEFAULT_DEG     60  // Default tilt angle for turret    

#define COXA_MIN_ANGLE_DEG          0   // Min angle for COXA servo
#define COXA_MAX_ANGLE_DEG          300 // Max angle for COXA servo
#define COXA_UP_ANGLE_DEG          150 // Default up angle for COXA servo
#define COXA_DOWN_ANGLE_DEG        100 // Default down angle for COXA servo 

#define FEMUR_MIN_ANGLE_DEG         0   // Min angle for FEMUR servo
#define FEMUR_MAX_ANGLE_DEG         300 // Max angle for FEMUR servo
#define FEMUR_UP_ANGLE_DEG         150 // Default up angle for FEMUR servo
#define FEMUR_DOWN_ANGLE_DEG       100 // Default down angle for FEMUR servo

#define TIBIA_MIN_ANGLE_DEG         0   // Min angle for TIBIA servo
#define TIBIA_MAX_ANGLE_DEG         300 // Max angle for TIBIA servo
#define TIBIA_UP_ANGLE_DEG         150 // Default up angle for TIBIA servo
#define TIBIA_DOWN_ANGLE_DEG       100 // Default down angle for TIBIA servo


// ------------------------
// GAIT TIMING
// ------------------------
#define DEFAULT_GAIT_CYCLE_MS 1000

#endif  // CONFIG_H