# SpiderBot

![MuthSpider](./img/MuthSpider.png)<br>

YouTube Video:
[![MuthSpider video](http://img.youtube.com/vi/tW0A7SIOCcE/default.jpg)](http://www.youtube.com/watch?v=tW0A7SIOCcE)
==========
   
   A hexapod spider robot controlled by [Robotis CM-700 controller](http://support.robotis.com/en/product/auxdevice/controller/cm700_manual.htm), motorized by twenty [Robotis Dynamixel AX-18A servos](http://support.robotis.com/en/product/dynamixel/ax_series/ax-18f.htm), and one [Robotis AX-S1 integrated sensor](http://support.robotis.com/en/product/auxdevice/sensor/dxl_ax_s1.htm) mounted on its head. The robot frame is a slightly modified TrossenRobotics ABS PhantomX kit.
 

# Robot Controller (Robotis CM-700):

There are many options of robotic controllers in the market and it is a great fun to try it all. Initially I started this spider project using the 1st version of TrossenRobotics Arbotix controller with the open source software from the same company. Then I tried the ROBOTIS OpenCM9.04 open source controller along with its Arduino like IDE. Finally, I picked ROBOTIS CM-700 controller along with ROBOTIS RoboPlus Software. I’m not sure how long I will keep using it.

![CM-700-1](./img/CM700-full.png)

What I like about CM-700 is its compact design, the low-power 8-bit AVR RISC-based microcontroller (ATMEL ATMEGA2561) which is widely used in many applications including Arduino boards, its big number of ports, and the very user friendly RoboPlus software.


![CM-700-2](./img/CM700-parts.png)

**Controller Specfications:**

* **MCU : ATMEL ATMega2561**
* **Working Temperature :** -5℃~70℃*
* **Weight :** 37.3g
* **Working Voltage:**
   * Allowed Range : 7V ~ 35V
* **Current:**
   - Consumed Current When idle  : 40mA
   - Exterior I/O Maximum Current : 0.9A
   - Overall Maximum Current : 10A (Fuse)
* **Internal I/O Device**
   - Button : 2 sets (Reset 1, Start 1)
   - Temperature Sensor : 1
   - Voltage Sensor : 1
* **External I/O Device**
   - OLLO 5 pin I/O Port : 6
   - 3-Pin Connector TTL Communication : 4
   - 4-Pin Connector RS-485 Communication : 5

# Robot Motors (Robotis AX-18A):

 
![AX-18A](./img/AX-18A.png)
 
# Robot Sensors (Robotis AX-S1):

 
![AX-S1](./img/AX-S1.png)

# Frame (Customised from PhantomX MK1):

# Battery:

# Programming Hardware (Robotis LN-101):

![LN-101](./img/LN-101.png)

# Programming Software (Robotis RoboPlus 1.x and 2.x):

![RoboPlus](./img/RoboPlus.png)

# Repository Contents

* **/Docs** - Project Related documentations.
* **/RoboPlus Files** - Motion,Task, and Design Files generated using Robotis RoboPlus 1.x & 2.x
* **/Research Paper** - Related robotics research documents.

#Version History

* Version 1.0 (work in progress).

# SpiderBot VSCode

## Overview

This project is a firmware for a hexapod robot ("SpiderBot") using Dynamixel AX-18A servos. The code is designed for Arduino-compatible microcontrollers and provides modular control for legs, a sensor turret, and optional AX-S1 sensor modules.

## Features

- **Hexapod Leg Control:** Modular support for 6 legs, each with multiple degrees of freedom.
- **Dynamixel Servo Communication:** Uses DynamixelWorkbench for robust servo management.
- **Sensor Turret:** Pan/tilt turret control for sensors or cameras.
- **RC Controller Support:** Remote control via serial interface.
- **Gait Controller:** Multiple walking gaits and speed control.
- **AX-S1 Sensor Integration:** Read temperature, luminosity, and sound level; control onboard LED.

## Directory Structure

```
HexapodPlatformIO/
├── include/           # Header files (e.g., AXS1Sensor.h)
├── src/               # Source files (e.g., main.cpp, AXS1Sensor.cpp)
├── lib/               # Optional libraries
├── platformio.ini     # PlatformIO project configuration
└── README.md          # Project documentation
```

## Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed (VS Code recommended)
- Supported Arduino-compatible board (e.g., Teensy, STM32, ESP32)
- Dynamixel servos and compatible power supply
- (Optional) AXS-1 sensor module

### Building and Uploading

1. Clone this repository:
    ```sh
    git clone <your-repo-url>
    cd HexapodPlatformIO
    ```

2. Open the project in VS Code with PlatformIO extension.

3. Connect your board via USB.

4. Build and upload the firmware:
    ```sh
    pio run --target upload
    ```

### Configuration

- Edit `Config.h` to set pin assignments, baud rates, and other hardware-specific parameters.

## Usage

- On startup, the robot initializes all modules and prints status messages to the serial monitor.
- The main loop demonstrates turret movement; you can expand this to add walking, sensor reading, or remote control.

## File Descriptions

- `src/main.cpp` - Main firmware entry point; initializes all modules and runs the main loop.
- `include/AXS1Sensor.h` / `src/AXS1Sensor.cpp` - Interface for the AX-S1 sensor module.
- `DynamixelController.*` - Handles low-level servo communication.
- `Hexapod.*`, `Leg.*` - Implements hexapod kinematics and leg control.
- `Turret.*` - Controls the pan/tilt sensor turret.
- `RCController.*` - Handles remote control input.
- `GaitController.*` - Manages walking gaits and timing.

## License

This project is provided for educational and research purposes. Please check individual source files for license details.

## Credits

- Uses [DynamixelWorkbench](http://emanual.robotis.com/docs/en/software/dynamixel/dynamixel_workbench/) library by ROBOTIS.
- Developed by Muthanna Alwahash (c) 2025
