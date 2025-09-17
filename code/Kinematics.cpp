#include "Kinematics.h"
#include "Console.h"
#include "Debug.h"

namespace IK {

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Utility Functions

    // Convert radians [M_PI, -M_PI] to degrees [180, -180]
    inline float    rad2Deg(float rad) {
        if (rad < -M_PI || rad > M_PI) {
            LOG_ERR("Angle out of range: " + String(rad) + " rad");
            return -1; // out of range
        }
        return rad * 180.0f / M_PI; 
    }

    // Convert degrees [-180, 180] to radians [-M_PI, M_PI]
    inline float    deg2Rad(float deg) {
        if (deg < -180.0f || deg > 180.0f) {
            LOG_ERR("Angle out of range: " + String(deg) + "°");
            return -1; // out of range
        }
        return deg * M_PI / 180.0f; 
    }

    // wrap angles from 180° to 360° (rotation and mirror).
    float wrap180to360(float deg) {
        if (deg < -180.0f || deg > 180.0f) {
            LOG_ERR("Angle out of range: " + String(deg) + "°");
            return -1; // out of range
        }
        return fmodf((180.0f - deg), 360.0f);
    }

    // wrap angles from 360° to 180° (rotation and mirror).
    float wrap360to180(float deg) {
        if (deg < 0.0f || deg > 360.0f) {
            LOG_ERR("Angle out of range: " + String(deg) + "°");
            return -1; // out of range
        }
        return 180.0f - deg;
    }

    // Convert degrees [30, 330] to ticks [0, 1023]
    bool deg2Tick(float deg, uint16_t &tick) {
        if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) {
            LOG_ERR("Angle out of range: " + String(deg) + "°");
            return false;
        }
        tick = static_cast<uint16_t>(lroundf(deg - SERVO_MIN_DEG) * (SERVO_MAX_TICK / SERVO_SPAN_DEG));
        return true;
    }

    // Convert ticks [0, 1023] to degrees [30, 330]
    bool tick2Deg(uint16_t tick, float &deg) {
        if (tick < SERVO_MIN_TICK || tick > SERVO_MAX_TICK) {
            LOG_ERR("Tick out of range: " + String(tick) + " Ticks");
            return false;
        }
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
        float coxa_angle_deg  = IK::wrap180to360(IK::rad2Deg(coxa_angle_rad) - baseR);      // Radians to degrees and wrap
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

        float femur_angle_rad = angle_a + KNEE_DIR * angle_b;                               // Femur angle
        float femur_angle_deg = IK::wrap180to360(IK::rad2Deg(femur_angle_rad));             // Radians to degrees and wrap
        if (!IK::deg2Tick(femur_angle_deg, positions[1])) {                                 // Degrees to ticks
            LOG_ERR("Femur angle out of range: " + String(femur_angle_deg) + "°");
            return false;
        }

        float cos_c = (powf(FEMUR_LENGTH, 2) + powf(TIBIA_LENGTH, 2) - powf(L, 2))          // Law of cosines for tibia angle
                    / (2.0f * FEMUR_LENGTH * TIBIA_LENGTH);
        cos_c = fminf(fmaxf(cos_c, -1.0f), 1.0f);                                           // Clamp for safety

        float tibia_angle_rad = KNEE_DIR * (M_PI - acosf(cos_c));                           // Tibia angle
        float tibia_angle_deg = IK::wrap180to360(IK::rad2Deg(tibia_angle_rad));             // Radians to degrees and wrap
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
        PRINTLN("Kinematics System Status:\n\r");
        PRINTLN("Coxa Length    : " + String(COXA_LENGTH) + " mm");
        PRINTLN("Femur Length   : " + String(FEMUR_LENGTH) + " mm");
        PRINTLN("Tibia Length   : " + String(TIBIA_LENGTH) + " mm");
        PRINTLN("");
        PRINTLN("Knee Direction : " + String(KNEE_DIR) + " (1: up, -1: down)");
        PRINTLN("");
        PRINTLN("Servo Range    : " + String(SERVO_MIN_DEG) + " to " + String(SERVO_MAX_DEG) + "°");
        PRINTLN("Servo Range    : " + String(SERVO_MIN_TICK) + " to " + String(SERVO_MAX_TICK) + " Ticks");
        PRINTLN("");
        return true;
    }

    // Process console commands for kinematics
    bool runConsoleCommands(const String& cmd, const String& args) {
        if (cmd == "ks") {
            printStatus();
            return true;

        } else if (cmd == "kf") {
            float baseX = 0.0f, baseY = 0.0f, baseZ = 0.0f, baseR = 0.0f;
            uint16_t coxa = 0, femur = 0, tibia = 0;
            if (sscanf(args.c_str(), "%hu %hu %hu", &coxa, &femur, &tibia) != 3) {
                LOG_ERR("Invalid arguments. Usage: kf C F T R");
                return false;
            }
            float tipX = 0.0f, tipY = 0.0f, tipZ = 0.0f;
            if (getFKGlobal(coxa, femur, tibia, baseX, baseY, baseZ, baseR, &tipX, &tipY, &tipZ)) {
                PRINTLN("FK Result - Tip Position: X=" + String(tipX) + " mm, Y=" + String(tipY) + " mm, Z=" + String(tipZ) + " mm");
            } else {
                LOG_ERR("FK computation failed.");
            }
            return true;

        } else if (cmd == "ki") {
            float tipX = 0.0f, tipY = 0.0f, tipZ = 0.0f, baseX = 0.0f, baseY = 0.0f, baseZ = 0.0f, baseR = 0.0f;
            if (sscanf(args.c_str(), "%f %f %f", &tipX, &tipY, &tipZ) != 3) {
                LOG_ERR("Invalid arguments. Usage: ki X Y Z R");
                return false;
            }
            uint16_t positions[3] = {0};
            if (getIKGlobal(tipX, tipY, tipZ, baseX, baseY, baseZ, baseR, positions)) {
                PRINTLN("IK Result - Servo Positions: Coxa=" + String(positions[0]) + " Ticks, Femur=" + String(positions[1]) + " Ticks, Tibia=" + String(positions[2]) + " Ticks");
            } else {
                LOG_ERR("IK computation failed.");
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
        PRINTLN("  kf C F T    - Perform forward kinematics");
        PRINTLN("  ki X Y Z    - Perform inverse kinematics");
        PRINTLN("  k?          - Print kinematics help information");
        PRINTLN("");
        return true;
    }

} // namespace IK
