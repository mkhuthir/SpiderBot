

#include <Arduino.h>                    // Include Arduino core library
#include "Config.h"                     // Include configuration header for serial ports and other settings     
#include "RC100.h"                      // Include Robotis RC100 remote controller library  
#include "Hexapod.h"                    // Include Hexapod class for managing the hexapod robot
#include "Turret.h"                     // Include Turret class for managing the sensor turret
#include "AXS1Sensor.h"                 // Include AXS1Sensor class for managing the AX-S1 sensor
#include "GaitController.h"             // Include GaitController class for managing the gait of the hexapod


// Global variables and instances
DynamixelController dxl;                // Initialize DynamixelWorkbench with the specified serial port and baud rate
RC100               rcCtrl;             // RC100 remote controller instance
Hexapod*            hexapod;            // Pointer to Hexapod instance
Turret*             turret;             // Pointer to Turret instance
AXS1Sensor*         sensor;             // Pointer to AXS1Sensor instance
GaitController*     gaitController;     // Pointer to GaitController instance

// Setup function to initialize the robot components
void setup() {

    // Initialize Serial for debugging
    Serial.begin(DEBUG_BAUD_RATE);
    while (!Serial); // Wait for Serial to be ready
    Serial.println("SpiderBot Starting Setup...");

    // Initialize Robotis RC100 Remote Controller
    rcCtrl.begin(RC100_SERIAL);     
    Serial.println("Robotis RC100 Remote Controller initialized.");

    // Initialize Dynamixel Controller
    dxl.begin(DXL_SERIAL, DXL_BAUD_RATE); 
    Serial.println("Dynamixel Controller initialized.");

    // Create Hexapod instance
    hexapod = new Hexapod(&dxl);                                    // Create Hexapod instance with Dynamixel controller
    hexapod->initialize();                                          // Initialize all legs
    hexapod->setGaitType(0);                                        // Set default gait
    hexapod->setGaitSpeed(0.5);                                     // Set default speed
    hexapod->setServoSpeed(0.5);                                    // Set default servo speed
    Serial.println("Dynamixel Controller initialized.");
    
    // Initialize Sensor Turret
    turret = new Turret(TURRET_PAN_ID, TURRET_TILT_ID, &dxl);       // Create Turret instance with Dynamixel controller
    turret->initialize();                                           // Initialize turret servos
    Serial.println("Sensor Turret initialized.");

    // Initialize AX-S1 Sensor
    sensor = new AXS1Sensor(dxl.getWorkbench(), AX_S1_SENSOR_ID);   // Create AX-S1 sensor instance with Dynamixel controller
    if (sensor->ping()) {
        Serial.println("AX-S1 detected!");
    } else {
        Serial.println("AX-S1 not detected!");
    }

    // Initialize Gait Controller
    gaitController = new GaitController(hexapod);                   // Create GaitController instance with Hexapod
    Serial.println("Gait Controller initialized.");

}

// Loop function to handle remote controller input and control the robot
void loop() {

  if (rcCtrl.available())
  {
    int RCRx = rcCtrl.readData();
    Serial.print("RCRx = ");
    Serial.print(RCRx);  

    if (RCRx & RC100_BTN_U) {
        Serial.println("U");
        turret->rotateTurretUp();    // Rotate turret up for testing
        
    } else if (RCRx & RC100_BTN_D) {
        Serial.println("D");
        turret->rotateTurretDown();  // Rotate turret down for testing

    } else if (RCRx & RC100_BTN_L) {
        Serial.println("L");
        turret->rotateTurretLeft();  // Rotate turret to the left for testing

    } else if (RCRx & RC100_BTN_R) {
        Serial.println("R");
        turret->rotateTurretRight(); // Rotate turret to the right for testing
        
    } else if (RCRx & RC100_BTN_1) {
        Serial.println("1");
        gaitController->setGait(GAIT_IDLE);

    } else if (RCRx & RC100_BTN_2) {
        Serial.println("2");
        gaitController->setGait(GAIT_WAVE);

    } else if (RCRx & RC100_BTN_3) {
        Serial.println("3");
        gaitController->setGait(GAIT_RIPPLE);

    } else if (RCRx & RC100_BTN_4) {
        Serial.println("4");
        gaitController->setGait(GAIT_TRIPOD);

    } else if (RCRx & RC100_BTN_5) {
        Serial.println("5");
        turret->rotateTurretHome();  // Rotate turret back to home position for testing

    } else if (RCRx & RC100_BTN_6) {
        Serial.println("6");

        hexapod->printLegsStatus();  // Print initial status of all legs
        turret->printTurretStatus(); // Print initial status of turret

        Serial.print("Temp: "); Serial.println(sensor->getTemperature());
        Serial.print("Voltage: "); Serial.println(sensor->getVoltage());
        
        Serial.print("Sound: "); Serial.println(sensor->getSoundLevel());
        Serial.print("Sound Count: "); Serial.println(sensor->getSoundCount());
        
        Serial.print("Light Right: "); Serial.println(sensor->getLightRight());
        Serial.print("Light Center: "); Serial.println(sensor->getLightCenter());
        Serial.print("Light Left: "); Serial.println(sensor->getLightLeft());   
        
        Serial.print("IR Right: "); Serial.println(sensor->getIRRight());   
        Serial.print("IR Center: "); Serial.println(sensor->getIRCenter());
        Serial.print("IR Left: "); Serial.println(sensor->getIRLeft());
        

        Serial.println("Sensor data read successfully.");

        // Bulk read
        Serial.println("Bulck reading postions");
        std::vector<uint32_t> positions;
        dxl.bulkReadPositions({1,2,3,4,5,6}, positions);
        for (auto p : positions) {
            Serial.println(p);
        }    
        
    }
  }

    gaitController->update();

    // Move all legs to neutral
    //std::vector<uint8_t> ids = {1,2,3,4,5,6};
    //std::vector<uint32_t> poses = {512,512,512,512,512,512};
    //dxl.syncWritePosition(ids, poses);
}
