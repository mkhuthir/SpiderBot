#ifndef CONFIG_H
#define CONFIG_H

// ------------------------
// SERIAL PORTS
// ------------------------
#define DXL_SERIAL_PORT   Serial3
#define RC_SERIAL_PORT    Serial1
#define DEBUG_SERIAL_PORT Serial

// ------------------------
// BAUD RATES
// ------------------------
#define DXL_BAUD_RATE     57600
#define RC_BAUD_RATE      57600
#define DEBUG_BAUD_RATE   115200

// ------------------------
// DYNAMIXEL PROTOCOL
// ------------------------
#define DXL_PROTOCOL_VERSION  1.0

// ------------------------
// SERVO LIMITS
// Define safe limits to avoid mechanical overrun
// AX-18A typical physical limit ~0-300 deg
#define SERVO_MIN_ANGLE_DEG   0
#define SERVO_MAX_ANGLE_DEG   300

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
// GAIT TIMING
// ------------------------
#define DEFAULT_GAIT_CYCLE_MS 1000

#endif
