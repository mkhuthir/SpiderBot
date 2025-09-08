#ifndef KINEMATICS_H
#define KINEMATICS_H

    #define COXA_LENGTH      float(52.0)                         // Length of the coxa segment in mm

    #define FEMUR_LENGTH     float(82.0)                         // Length of the femur segment in mm
    #define FEMUR_H_POS      float(180.0)                        // Femur at horizontal angle in degrees
    #define FEMUR_UP_DIR     float(1.0)                        // Femur up direction CCW = 1.0 CW = -1.0
    
    #define TIBIA_LENGTH     float(142.0)                        // Length of the tibia segment in mm
    #define TIBIA_H_POS      float(180.0)                        // Tibia at horizontal angle in degrees
    #define TIBIA_UP_DIR     float(-1.0)                       // Tibia up direction CCW = 1.0 CW = -1.0

    #define SERVO_MIN_DEG    float(30.0)                          // start angle
    #define SERVO_MAX_DEG    float(330.0)                         // end angle
    #define SERVO_SPAN_DEG   (SERVO_MAX_DEG - SERVO_MIN_DEG)    // 300 degrees span (30-300 CCW)
    #define SERVO_MIN_TICK   uint16_t(0)                        // min ticks 0 = 30 degrees
    #define SERVO_MAX_TICK   uint16_t(1023)                     // max ticks 1023 = 300 degrees

    #include <Arduino.h>

    namespace IK {

        // Utility Functions
        inline float    rad2Deg(float rad) { return rad * 180.0f / M_PI; }              // Convert radians [M_PI, -M_PI] to degrees [180, -180]
        inline float    deg2Rad(float deg) { return deg * M_PI / 180.0f; }              // Convert degrees [-180, 180] to radians [-M_PI, M_PI]
        float           wrap360(float deg);                                             // Wrap angle from [180, -180] to [0, 360]
        bool            deg2Tick(float deg, uint16_t &tick);                            // Convert degrees [30, 330] to ticks [0, 1023]
        bool            tick2Deg(uint16_t tick, float &deg);                            // Convert ticks [0, 1023] to degrees [30, 330]

        // Coordinate Transformations from leg local coordinates to body global coordinates and vise versa
        void            global2Local(float global_x, float global_y, float global_z, float baseX, float baseY, float baseZ, float* local_x, float* local_y, float* local_z);
        void            local2Global(float local_x, float local_y, float local_z, float baseX, float baseY, float baseZ, float* global_x, float* global_y, float* global_z);

        // Inverse Kinematics local and global
        bool            getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, float baseR, uint16_t* positions);
        bool            getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, float baseX, float baseY, float baseZ, float baseR, uint16_t* positions);

        // Forward Kinematics local and global
        bool            getFKLocal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseR, float* tip_local_x, float* tip_local_y, float* tip_local_z);
        bool            getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseX, float baseY, float baseZ, float baseR, float* tip_global_x, float* tip_global_y, float* tip_global_z);

        // Console command handlers
        bool      printStatus();                                                        // Print the status of the kinematics system
        bool      runConsoleCommands(const String& cmd, const String& args);            // Process console commands for kinematics
        bool      printConsoleHelp();                                                   // Print kinematics-specific help information

    }

#endif // KINEMATICS_H