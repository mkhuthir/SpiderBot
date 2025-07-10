

#include <Arduino.h>
#include "Config.h"
#include "DynamixelController.h"
#include "Leg.h"
#include "Hexapod.h"
#include "Turret.h"
#include "AXS1Sensor.h"
#include "GaitController.h"


DynamixelController dxlController(DXL_SERIAL, DXL_DIR_PIN); // Create an instance of DynamixelController with the specified serial port and direction pin
RCController rcController(RC_SERIAL);                       // Create an instance of RCController with the specified serial port

DynamixelWorkbench dxl;             // DynamixelWorkbench instance for advanced control

Hexapod*        hexapod;            // Pointer to Hexapod instance
Turret*         turret;             // Pointer to Turret instance
AXS1Sensor*     sensor;             // Pointer to AXS1Sensor instance
GaitController* gaitController;     // Pointer to GaitController instance

void setup() {

  // Initialize Serial for debugging
  Serial.begin(DEBUG_BAUD_RATE);
  while (!Serial); // Wait for Serial to be ready

  Serial.println("SpiderBot Starting Setup...");

  // Initialize Dynamixel Controller
  dxlController.begin(DXL_BAUD_RATE, DXL_PROTOCOL_VERSION);

  if (!dxlController.isConnected()) {
    Serial.println("Failed to connect to Dynamixel servos.");   // Print error message
    while (true);                                               // Halt execution if connection fails
  }
  Serial.println("Dynamixel Controller connected successfully.");

  // Initialize Hexapod
  hexapod = new Hexapod(&dxlController);  // Create Hexapod instance with Dynamixel controller
  hexapod->initialize();                  // Initialize all legs
  
  hexapod->setGaitType(0);                // Set default gait
  hexapod->setGaitSpeed(0.5);             // Set default speed
  hexapod->setServoSpeed(0.5);            // Set default servo speed

  Serial.println("Dynamixel Controller initialized.");
  
  // Initialize Sensor Turret
  turret = new Turret(TURRET_PAN_ID, TURRET_TILT_ID, &dxlController); // Create Turret instance with Dynamixel controller
  turret->initialize();                                               // Initialize turret servos
  turret->rotateTurretHome();                                         // Rotate turret to home position
  Serial.println("Sensor Turret initialized.");
  
  // Initialize Gait Controller
  gaitController = new GaitController(hexapod, DEFAULT_GAIT_CYCLE_MS);
  Serial.println("Gait Controller initialized.");
  Serial.println("SpiderBot Setup Complete.");
  Serial.println("Ready to receive commands.");

  // Initialize AX-S1 Sensor
  dxl.begin(DXL_SERIAL, DXL_BAUD_RATE);           // Initialize DynamixelWorkbench with the specified serial port and baud rate
  sensor = new AXS1Sensor(&dxl, AX_S1_SENSOR_ID); // Create AX-S1 sensor instance with Dynamixel controller
  if (sensor->ping()) {
      Serial.println("AX-S1 detected!");
  } else {
      Serial.println("AX-S1 not detected!");
  }

  hexapod->printLegsStatus();  // Print initial status of all legs
  turret->printTurretStatus(); // Print initial status of turret

}

void loop() {

    remote.update();
    uint16_t command = remote.getCommand();

    if (command & RC100_BTN_U) {
        Serial.println("U");
        turret->rotateTurretUp();    // Rotate turret up for testing
        
    } else if (command & RC100_BTN_D) {
        Serial.println("D");
        turret->rotateTurretDown();  // Rotate turret down for testing

    } else if (command & RC100_BTN_L) {
        Serial.println("L");
        turret->rotateTurretLeft();  // Rotate turret to the left for testing

    } else if (command & RC100_BTN_R) {
        Serial.println("R");
        turret->rotateTurretRight(); // Rotate turret to the right for testing
        
    } else if (command & RC100_BTN_1) {
        Serial.println("Idle mode");
        //gaitController->setGait(GAIT_IDLE);

    } else if (command & RC100_BTN_2) {
        Serial.println("Wave gait selected");
        //gaitController->setGait(GAIT_WAVE);

    } else if (command & RC100_BTN_3) {
        Serial.println("Ripple gait selected");
        //gaitController->setGait(GAIT_RIPPLE);

    } else if (command & RC100_BTN_4) {
        Serial.println("Tripod gait selected");
        //gaitController->setGait(GAIT_TRIPOD);

    } else if (command & RC100_BTN_5) {
        Serial.println("tURRET HOME");
        turret->rotateTurretHome();  // Rotate turret back to home position for testing

    } else if (command & RC100_BTN_6) {
        Serial.print("Temp: "); Serial.println(sensor->readTemperature());
        Serial.print("Light: "); Serial.println(sensor->readLuminosity());
        Serial.print("Sound: "); Serial.println(sensor->readSoundLevel());
        
    }

    //gaitController->update();
    
   
  
  delay(1000); // Wait for 1 second before the next loop iteration
}
