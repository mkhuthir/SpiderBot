#include "Kinematics.h"
#include "Console.h"
#include "Debug.h"

namespace IK {

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Utility Functions

    // Convert radians [M_PI, -M_PI] to degrees [180, -180]
    inline float    rad2Deg(float rad) {
        return rad * 180.0f / M_PI; 
    }

    // Convert degrees [-180, 180] to radians [-M_PI, M_PI]
    inline float    deg2Rad(float deg) {
        return deg * M_PI / 180.0f; 
    }

    // 180° rotation and mirror.
    float wrap360(float deg) {
        return fmodf((180.0f - deg), 360.0f);
    }

    // Convert degrees [30, 330] to ticks [0, 1023]
    bool deg2Tick(float deg, uint16_t &tick) {
        if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) return false;
        float t = (deg - SERVO_MIN_DEG) * (SERVO_MAX_TICK / SERVO_SPAN_DEG);
        if (t < SERVO_MIN_TICK) t = SERVO_MIN_TICK;
        if (t > SERVO_MAX_TICK) t = SERVO_MAX_TICK;
        tick = static_cast<uint16_t>(lroundf(t));
        return true;
    }

    // Convert ticks [0, 1023] to degrees [30, 330]
    bool tick2Deg(uint16_t tick, float &deg) {
        if (tick < SERVO_MIN_TICK || tick > SERVO_MAX_TICK) return false;
        deg = SERVO_MIN_DEG + (static_cast<float>(tick) * SERVO_SPAN_DEG / SERVO_MAX_TICK);
        return true;
    }

    // Coordinate Transformations from body global coordinates to leg local coordinates
    void global2Local(float global_x, float global_y, float global_z, float baseX, float baseY, float baseZ, float* local_x, float* local_y, float* local_z) {
        *local_x = global_x - baseX;
        *local_y = global_y - baseY;
        *local_z = global_z - baseZ;
    }

    // Coordinate Transformations from leg local coordinates to body global coordinates
    void local2Global(float local_x, float local_y, float local_z, float baseX, float baseY, float baseZ, float* global_x, float* global_y, float* global_z) {
        *global_x = baseX + local_x;
        *global_y = baseY + local_y;
        *global_z = baseZ + local_z;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Local Inverse Kinematics
    bool getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, float baseR, uint16_t* positions) {
        
        float coxa_angle_rad = atan2f(tip_local_y, tip_local_x);                            // Coxa yaw (rotation in XY plane)
        float coxa_angle_deg  = IK::wrap360(IK::rad2Deg(coxa_angle_rad) - baseR);           // Radians to degrees and wrap
        if (!IK::deg2Tick(coxa_angle_deg,  positions[0])) {                                 // Degrees to ticks
            LOG_ERR("Coxa angle out of range: " + String(coxa_angle_deg) + "°");
            return false;                     
        }

        // Femur and Tibia angles
        float planar_dist = hypotf(tip_local_x, tip_local_y) - COXA_LENGTH;                 // Compute planar distance from coxa axis to tip (in XY plane)
        float L = hypotf(planar_dist, tip_local_z);                                         // Law of cosines for femur and tibia
        if (L > (FEMUR_LENGTH + TIBIA_LENGTH) || L < fabsf(FEMUR_LENGTH - TIBIA_LENGTH)) {  // Reachability check
            LOG_ERR("Target out of reach: L=" + String(L) + "mm");
            return false;
        }

        float angle_a = atan2f(tip_local_z, planar_dist);                                   // Angle from coxa axis to tip in the leg plane
        float cos_b = (powf(FEMUR_LENGTH, 2) + powf(L, 2) - powf(TIBIA_LENGTH, 2))          // Law of cosines for femur angle
                    / (2.0f * FEMUR_LENGTH * L);  
        cos_b = fminf(fmaxf(cos_b, -1.0f), 1.0f);                                           // Clamp for safety
        float angle_b = acosf(cos_b);                                                       // Angle between femur and line from coxa to tip
        float knee_sign = -1.0f;                                                            // Choose knee sign (default, knee up > negative sign)

        float femur_angle_rad = angle_a + knee_sign * angle_b;                              // Femur angle
        float femur_angle_deg = IK::wrap360(IK::rad2Deg(femur_angle_rad));                  // Radians to degrees and wrap
        if (!IK::deg2Tick(femur_angle_deg, positions[1])) {                                 // Degrees to ticks
            LOG_ERR("Femur angle out of range: " + String(femur_angle_deg) + "°");
            return false;
        }

        float cos_c = (powf(FEMUR_LENGTH, 2) + powf(TIBIA_LENGTH, 2) - powf(L, 2))          // Law of cosines for tibia angle
                    / (2.0f * FEMUR_LENGTH * TIBIA_LENGTH);
        cos_c = fminf(fmaxf(cos_c, -1.0f), 1.0f);                                           // Clamp for safety

        float tibia_angle_rad = knee_sign * (M_PI - acosf(cos_c));                          // Tibia angle  
        float tibia_angle_deg = IK::wrap360(IK::rad2Deg(tibia_angle_rad));                  // Radians to degrees and wrap
        if (!IK::deg2Tick(tibia_angle_deg, positions[2])) {                                 // Degrees to ticks
            LOG_ERR("Tibia angle out of range: " + String(tibia_angle_deg) + "°");
            return false;
        }

        return true;
    }

    // Global Inverse Kinematics
    bool getIKGlobal(float tip_global_x, float tip_global_y, float tip_global_z, float baseX, float baseY, float baseZ, float baseR, uint16_t* positions) {
        float tip_local_x, tip_local_y, tip_local_z;
        global2Local(tip_global_x, tip_global_y, tip_global_z, baseX, baseY, baseZ, &tip_local_x, &tip_local_y, &tip_local_z);
        return getIKLocal(tip_local_x, tip_local_y, tip_local_z, baseR, positions);
    }

    // Local Forward Kinematics
    bool getFKLocal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseR, float* tip_local_x, float* tip_local_y, float* tip_local_z) {
        // Convert servo positions to angles in degrees
        float coxa_angle_deg  = coxa  * (300.0f / 1023.0f);
        float femur_angle_deg = femur * (300.0f / 1023.0f);
        float tibia_angle_deg = tibia * (300.0f / 1023.0f);

        // Convert degrees to radians
        float coxa_angle_rad  = coxa_angle_deg  * M_PI / 180.0f + baseR;
        float femur_angle_rad = femur_angle_deg * M_PI / 180.0f;
        float tibia_angle_rad = tibia_angle_deg * M_PI / 180.0f;

        // Planar FK for femur and tibia
        float planar_length = FEMUR_LENGTH * cos(femur_angle_rad) + TIBIA_LENGTH * cos(femur_angle_rad + tibia_angle_rad);
        float z = FEMUR_LENGTH * sin(femur_angle_rad) + TIBIA_LENGTH * sin(femur_angle_rad + tibia_angle_rad);

        // Tip position in leg base frame
        *tip_local_x = COXA_LENGTH + planar_length * cos(coxa_angle_rad);
        *tip_local_y = planar_length * sin(coxa_angle_rad);
        *tip_local_z = z;

        return true;
    }

    // Global Forward Kinematics
    bool getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseX, float baseY, float baseZ, float baseR, float* tip_global_x, float* tip_global_y, float* tip_global_z) {
        float tip_local_x = 0.0f, tip_local_y = 0.0f, tip_local_z = 0.0f;
        // Compute local tip position using FK
        if (!getFKLocal(coxa, femur, tibia, baseR, &tip_local_x, &tip_local_y, &tip_local_z)) {
            return false;
        }
        local2Global(tip_local_x, tip_local_y, tip_local_z, baseX, baseY, baseZ, tip_global_x, tip_global_y, tip_global_z);
        return true;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Console command handlers

    // Print kinematics status
    bool printStatus() {
        PRINTLN("Kinematics System Status:");
        PRINTLN("COXA_LENGTH  : " + String(COXA_LENGTH) + " mm");
        PRINTLN("FEMUR_LENGTH : " + String(FEMUR_LENGTH) + " mm");
        PRINTLN("TIBIA_LENGTH : " + String(TIBIA_LENGTH) + " mm");
        PRINTLN("Servo Range  : " + String(SERVO_MIN_DEG) + "° to " + String(SERVO_MAX_DEG) + "°"); 
        return true;
    }

    // Process console commands for kinematics
    bool runConsoleCommands(const String& cmd, const String& args) {
        if (cmd == "ks") {
            printStatus();
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

        } else if (cmd == "kgikg") {
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

        } else if (cmd == "kgfkl") {
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

        } else if (cmd == "kgfkg") {
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

        } else if (cmd == "kltg") {
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
            
        } else if (cmd == "kgtl") {
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
    
        } else if (cmd == "k?") {
            printConsoleHelp();
            return true;
        }
        return false;
    }   

    // Print kinematics-specific help information
    bool printConsoleHelp() {
        PRINTLN("Kinematics Commands:\n\r");
        PRINTLN("  ks          - Print kinematics system status");
        PRINTLN("");
        PRINTLN("  kgikl n x y z      - Compute IK in local coords (relative to leg base)");
        PRINTLN("  kgikg n x y z      - Compute IK in global coords (relative to body center)");
        PRINTLN("");
        PRINTLN("  kgfkl n c f t      - Compute FK in local coords (relative to leg base)");
        PRINTLN("  kgfkg n c f t      - Compute FK in global coords (relative to body center)");
        PRINTLN("");
        PRINTLN("  kltg n x y z       - Compute global coordinates from local coordinates");
        PRINTLN("  kgtl n x y z       - Compute local coordinates from global coordinates");
        PRINTLN("");
        PRINTLN("  k?                 - Show this help");
        PRINTLN("");
        return true;
    }

} // namespace IK
