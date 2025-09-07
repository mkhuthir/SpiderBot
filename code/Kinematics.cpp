#include "Kinematics.h"
#include "Console.h"
#include "Debug.h"

namespace IK {

    // 180° rotation and mirror.
    float wrap360(float deg) {
        return fmodf((180.0f - deg), 360.0f);
    }

    bool deg2Tick(float deg, uint16_t &tick) {
        if (deg < SERVO_MIN_DEG || deg > SERVO_MAX_DEG) return false;
        float t = (deg - SERVO_MIN_DEG) * (SERVO_MAX_TICK / SERVO_SPAN_DEG);
        if (t < SERVO_MIN_TICK) t = SERVO_MIN_TICK;
        if (t > SERVO_MAX_TICK) t = SERVO_MAX_TICK;
        tick = static_cast<uint16_t>(lroundf(t));
        return true;
    }

    bool tick2Deg(uint16_t tick, float &deg) {
        if (tick < SERVO_MIN_TICK || tick > SERVO_MAX_TICK) return false;
        deg = SERVO_MIN_DEG + (static_cast<float>(tick) * SERVO_SPAN_DEG / SERVO_MAX_TICK);
        return true;
    }

    void global2Local(float global_x, float global_y, float global_z, float baseX, float baseY, float baseZ, float* local_x, float* local_y, float* local_z) {
        *local_x = global_x - baseX;
        *local_y = global_y - baseY;
        *local_z = global_z - baseZ;
    }

    void local2Global(float local_x, float local_y, float local_z, float baseX, float baseY, float baseZ, float* global_x, float* global_y, float* global_z) {
        *global_x = baseX + local_x;
        *global_y = baseY + local_y;
        *global_z = baseZ + local_z;
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------------------
    // Local Inverse Kinematics
    bool getIKLocal(float tip_local_x, float tip_local_y, float tip_local_z, float baseR, uint16_t* positions) {

       
        float coxa_angle_rad = atan2f(tip_local_y, tip_local_x);                        // Coxa yaw (rotation in XY plane)
        float coxa_angle_deg  = IK::wrap360(IK::rad2Deg(coxa_angle_rad) - baseR);       // Radians to degrees and wrap
        LOG_INF("Coxa Angle: " + String(coxa_angle_deg) + "°");
        if (!IK::deg2Tick(coxa_angle_deg,  positions[0])) return false;                 // Degrees to ticks    
        LOG_INF("Coxa Tick: " + String(positions[0]));

        femur_angle_rad = 0.0f; // TODO: Implement femur IK calculations
        tibia_angle_rad = 0.0f; // TODO: Implement tibia IK calculations

        // Radians to degrees, wrap, and convert to ticks
        float femur_angle_deg = IK::wrap360(IK::rad2Deg(femur_angle_rad));
        LOG_INF("Femur Angle: " + String(femur_angle_deg) + "°");
        if (!IK::deg2Tick(femur_angle_deg, positions[1])) return false;
        LOG_INF("Femur Tick: " + String(positions[1]));

        // Radians to degrees, wrap, and convert to ticks
        float tibia_angle_deg = IK::wrap360(IK::rad2Deg(tibia_angle_rad));
        LOG_INF("Tibia Angle: " + String(tibia_angle_deg) + "°");
        if (!IK::deg2Tick(tibia_angle_deg, positions[2])) return false;
        LOG_INF("Tibia Tick: " + String(positions[2]));

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

    bool getFKGlobal(uint16_t coxa, uint16_t femur, uint16_t tibia, float baseX, float baseY, float baseZ, float baseR, float* tip_global_x, float* tip_global_y, float* tip_global_z) {
        float tip_local_x = 0.0f, tip_local_y = 0.0f, tip_local_z = 0.0f;
        // Compute local tip position using FK
        if (!getFKLocal(coxa, femur, tibia, baseR, &tip_local_x, &tip_local_y, &tip_local_z)) {
            return false;
        }
        local2Global(tip_local_x, tip_local_y, tip_local_z, baseX, baseY, baseZ, tip_global_x, tip_global_y, tip_global_z);
        return true;
    }

} // namespace IK
