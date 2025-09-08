#ifndef CONSOLE_H
#define CONSOLE_H

    #include <Arduino.h>
    
    #include "CommandHistory.h"     // Include CommandHistory class for command history management
    #include "Debug.h"              // Include Log class for logging
    #include "Driver.h"             // Include Driver class for managing Dynamixel servos
    #include "Servo.h"              // Include Servo class for managing servo motors
    #include "Kinematics.h"        // Include Kinematics class for kinematic calculations
    #include "Microcontroller.h"    // Include Microcontroller class for managing the microcontroller
    #include "Hexapod.h"            // Include Hexapod class for managing the hexapod robot
    #include "BodyPose.h"           // Include BodyPose class for managing body pose
    #include "Turret.h"             // Include Turret class for managing the sensor turret
    #include "AXS1Sensor.h"         // Include AXS1Sensor class for sensor management
    #include "GaitController.h"     // Include GaitController for movement control
    #include "Remotecontroller.h"   // Include RemoteController class for remote control input

    class Console {
        public:
            Console(    Stream*             stream,                 // Initialize the console with a stream
                        unsigned long       baud,                   // Baud rate for serial communication
                        Microcontroller*    mc      = nullptr,      // Pointer to Microcontroller instance
                        Driver*             driver  = nullptr,      // Pointer to Driver instance
                        Servo*              servo   = nullptr,      // Pointer to Servo instance
                        Hexapod*            hexapod = nullptr,      // Pointer to Hexapod instance
                        BodyPose*           bodyPose = nullptr,      // Pointer to BodyPose instance
                        Turret*             turret  = nullptr,      // Pointer to Turret instance
                        AXS1Sensor*         sensor  = nullptr,      // Pointer to AXS1Sensor instance
                        GaitController*     gc      = nullptr,      // Pointer to GaitController instance
                        Remotecontroller*   rc      = nullptr       // Pointer to RemoteController instance
            );

            bool begin();                                           // Initialize the console
            bool startShell();                                      // Start the shell
            bool update();                                          // Call in loop()

        private:
            Stream*             stream;                             // Pointer to the stream for input/output
            String              inputBuffer     = "";               // Buffer to store the input command
            String              shell           = "$";              // Shell prompt string
            int                 cursorPos       = 0;                // Current cursor position in input buffer
            bool                insertMode      = true;             // Insert mode flag (true=insert, false=overwrite)
            CommandHistory      commandHistory;                     // Command history management
            
            // Pointers to instances
            Microcontroller*    mc;                                 // Pointer to Microcontroller instance
            Driver*             driver;                             // Pointer to Driver instance
            Servo*              servo;                              // Pointer to Servo instance for Dynamixel control
            Hexapod*            hexapod;                            // Pointer to hexapod instance
            Turret*             turret;                             // Pointer to turret instance  
            BodyPose*           bodyPose;                           // Pointer to BodyPose instance
            AXS1Sensor*         sensor;                             // Pointer to AXS1Sensor instance (can be nullptr)
            GaitController*     gc;                                 // Pointer to GaitController instance
            Remotecontroller*   rc;                                 // Pointer to RemoteController instance

            // Input processing methods
            void processInput(const String& input);                 // Process the input entered by the user
            void handlePrintableChar(char c);                       // Handle printable character input and display
            void handleInputControl(char c);                        // Handle all input control operations
            
            // Escape sequence handlers
            void handleEscapeSequence();                            // Main escape sequence handler
            void handleCSISequence();                               // Control Sequence Introducer (ESC[)
            void handleSSSequence();                                // Single Shift sequence (ESC-O)
            void handleExtendedKey(char keyCode);                   // Extended keys ending with '~'
            
            // Arrow key handlers
            void handleArrowUp();                                   // Up arrow - command history previous
            void handleArrowDown();                                 // Down arrow - command history next
            void handleArrowRight();                                // Right arrow - move cursor right
            void handleArrowLeft();                                 // Left arrow - move cursor left
            
            // Navigation key handlers
            void handleHome();                                      // Home key - move to beginning
            void handleEnd();                                       // End key - move to end
            
            // Edit key handlers
            void handleInsertKey();                                 // Insert key - toggle insert/overwrite
            void handleDeleteKey();                                 // Delete key - delete character at cursor
            void handleBackspace();                                 // Backspace - delete character before cursor
            
            // Control key handlers
            void handleClearScreen();                               // Ctrl-L - clear screen
            void handleNewline();                                   // Enter - process command
            
            // Input helper methods
            void clearAndRedrawLine(const String& newContent);     // Clear line and redraw with new content
            void refreshLineFromCursor();                           // Refresh display from cursor position
            void resetInputState();                                 // Reset input state after command

            // Console Command Execution and Help
            bool runConsoleCommands(const String& cmd, const String& args);     // System commands (help, cls, status)
            bool runLegCommand(const String& cmd, const String& args);          // Leg control commands
            bool printAllHelp();                                       // Print comprehensive help information
            bool printConsoleHelp();                                // Print console-specific help information
        

    };

#endif
