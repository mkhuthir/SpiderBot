#include "Console.h"
#include "Debug.h"

// Constructor for Console class
Console::Console(   Stream* stream,
                    unsigned long baud,
                    Microcontroller* mc,
                    Driver* driver,
                    Servo* servo,
                    Hexapod* hexapod,
                    BodyPose* bodyPose,
                    Turret* turret,
                    AXS1Sensor* sensor,
                    GaitController* gc,
                    Remotecontroller* rc
                    ){

    log::setLogStream(stream);                // Set the log stream to the same stream
    this->stream = stream;

    if (stream == &Serial) {
        Serial.begin(baud);
    }
    
    this->mc        = mc;               // Store the Microcontroller instance
    this->driver    = driver;           // Store the Driver instance
    this->servo     = servo;            // Store the Servo instance
    this->hexapod   = hexapod;          // Store the hexapod instance
    this->turret    = turret;           // Store the turret instance
    this->bodyPose  = bodyPose;         // Store the BodyPose instance
    this->sensor    = sensor;           // Store the AXS1Sensor instance
    this->gc        = gc;               // Store the GaitController instance
    this->rc        = rc;               // Store the RemoteController instance

    shell           = "$";              // Default shell prompt
    cursorPos       = 0;                // Start cursor at position 0
    insertMode      = true;             // Default to insert mode
}

// Initialize the console with a baud rate and instances of components
bool Console::begin() {
    
    if (stream == nullptr) {
        LOG_ERR("Log stream is not initialized.");
        return false;
    }

    if (stream == &Serial) {
        while (!Serial);                            // Wait for Serial if using USB
    }

    PRINT("\033[2J\033[H");                         // ANSI escape code: clear screen and move cursor to top-left
    PRINTLN("SpiderBot Firmware v1.0 (c) 2025");
    LOG_INF("Console initialized successfully.");   // Log initialization message
    return true;
}

// Start the shell
bool Console::startShell() {
    PRINTLN("Starting SpiderBot control shell...");
    PRINTLN("Type '?' for help.");      // Print help message
    commandHistory.resetToEnd();        // Reset command history to end
    PRINT(shell);                       // Print the shell prompt
    return true;
}

// Update the console, reading input and processing commands
bool Console::update() {
    while (stream->available()) {
        char c = stream->read();            // Read a character from the console input

        if (c >= 32 && c <= 126) {      // Printable ASCII characters
            handlePrintableChar(c);
        } else {
            handleInputControl(c);      // Control characters and special sequences
        }
    }
    return true;
}

// Process the command entered by the user
void Console::processInput(const String& input) {
    
    String cmd = input;
    String mainCmd, args;

    cmd.trim();                                     // Remove leading/trailing whitespace
    cmd.toLowerCase();                              // Convert to lowercase for consistent handling
    if (cmd.length() == 0) return;                  // Skip empty commands
    commandHistory.addCommand(input);               // Add original command to history
    int spaceIndex = cmd.indexOf(' ');              // Find the first space character
    if (spaceIndex != -1) {                         // If a space is found
        mainCmd = cmd.substring(0, spaceIndex);     // Extract main command
        args = cmd.substring(spaceIndex + 1);       // Extract arguments
        args.trim();                                // Remove leading/trailing whitespace

    } else {
        mainCmd = cmd;
    }

    // Execute command using organized command handlers
    if (!runConsoleCommands(mainCmd, args) &&
        !mc->runConsoleCommands(mainCmd, args) &&
        !driver->runConsoleCommands(mainCmd, args) &&
        !servo->runConsoleCommands(mainCmd, args) &&
        !IK::runConsoleCommands(mainCmd, args) &&
        !hexapod->runConsoleCommands(mainCmd, args) &&
        !runLegCommand(mainCmd, args) &&                    
        !turret->runConsoleCommands(mainCmd, args) &&
        !bodyPose->runConsoleCommands(mainCmd, args) &&
        !sensor->runConsoleCommands(mainCmd, args) &&
        !gc->runConsoleCommands(mainCmd, args) &&
        !rc->runConsoleCommands(mainCmd, args)) {
            LOG_ERR("Unknown command: " + input);   // Unknown command
            PRINTLN("Type '?' for help.");
        }
}

// Handle printable character input and display
void Console::handlePrintableChar(char c) {
    if (insertMode) {
        if (cursorPos < (int)inputBuffer.length()) {            // Insert mode: insert character at cursor position
            String before = inputBuffer.substring(0, cursorPos);    // Insert character in the middle of the string
            String after = inputBuffer.substring(cursorPos);
            inputBuffer = before + c + after;
            PRINT("\033[K");                                // Clear from cursor to end and reprint
            String remaining = inputBuffer.substring(cursorPos);
            PRINT(remaining);
            for (int i = remaining.length() - 1; i > 0; i--) {  // Move cursor back to position after inserted character
                PRINT("\033[D");
            }
        } else {
            inputBuffer += c;                                   // At end of string, just append and echo
            stream->write(c);
        }
    } else {
        if (cursorPos < (int)inputBuffer.length()) {            // Overwrite mode: replace character at cursor position
            inputBuffer.setCharAt(cursorPos, c);                // Replace character at cursor position
            stream->write(c);                                       // Echo the character
        } else {
            inputBuffer += c;                                   // At end of string, append new character
            stream->write(c);                                       // Echo the character
        }
    }
    cursorPos++;
}

// Handle all input control operations (escape sequences, backspace, newlines, etc.)
void Console::handleInputControl(char c) {
    switch (c) {
        case '\x1B':                                            // Escape character (start of escape sequence)
            handleEscapeSequence();
            break;
            
        case '\b':                                              // Backspace
        case '\x7F':                                            // DEL character (some terminals)
            handleBackspace();
            break;
            
        case '\x0C':                                            // Ctrl-L (clear screen)
            handleClearScreen();
            break;
            
        case '\n':                                              // Newline
        case '\r':                                              // Carriage return
            handleNewline();
            break;
            
        case '\t':                                              // Tab character (ignore for now)
            // Could be extended for command completion
            break;
            
        default:
            // Ignore other control characters
            break;
    }
}

// Helper method to handle escape sequences with timeout protection
void Console::handleEscapeSequence() {
    // Wait for next character with timeout (100ms)
    unsigned long timeout = millis() + 100;
    while (!stream->available() && millis() < timeout);

    if (!stream->available()) return;                               // Timeout - ignore incomplete sequence

    char c1 = stream->read();
    if (c1 == '[') {
        handleCSISequence();                                    // Control Sequence Introducer
    } else if (c1 == 'O') {
        handleSSSequence();                                     // Single Shift sequence (function keys)
    }
    // Ignore other escape sequences
}

// Handle CSI (Control Sequence Introducer) sequences like ESC[A, ESC[2~, etc.
void Console::handleCSISequence() {
    unsigned long timeout = millis() + 100;
    while (!stream->available() && millis() < timeout);

    if (!stream->available()) return;                               // Timeout

    char c2 = stream->read();
    switch (c2) {
        case 'A':                                               // Up arrow - Command history: previous
            handleArrowUp();
            break;
            
        case 'B':                                               // Down arrow - Command history: next
            handleArrowDown();
            break;
            
        case 'C':                                               // Right arrow - Move cursor right
            handleArrowRight();
            break;
            
        case 'D':                                               // Left arrow - Move cursor left
            handleArrowLeft();
            break;
            
        case 'H':                                               // Home key
            handleHome();
            break;
            
        case 'F':                                               // End key
            handleEnd();
            break;
            
        case '1':                                               // Function keys F1-F4 (ESC[1~)
        case '2':                                               // Insert key (ESC[2~)
        case '3':                                               // Delete key (ESC[3~)
        case '4':                                               // End key alternative (ESC[4~)
        case '5':                                               // Page Up (ESC[5~)
        case '6':                                               // Page Down (ESC[6~)
            handleExtendedKey(c2);
            break;
            
        default:
            // Ignore unknown sequences
            break;
    }
}

// Handle SS (Single Shift) sequences like ESC-O for function keys
void Console::handleSSSequence() {
    unsigned long timeout = millis() + 100;
    while (!stream->available() && millis() < timeout);

    if (!stream->available()) return;                               // Timeout

    char c2 = stream->read();
    switch (c2) {
        case 'H':                                               // Home key (alternative)
            handleHome();
            break;
            
        case 'F':                                               // End key (alternative)
            handleEnd();
            break;
            
        case 'P':                                               // F1 key
        case 'Q':                                               // F2 key
        case 'R':                                               // F3 key
        case 'S':                                               // F4 key
            // Function keys - could be used for shortcuts in the future
            break;
            
        default:
            // Ignore unknown sequences
            break;
    }
}

// Handle extended keys that end with '~'
void Console::handleExtendedKey(char keyCode) {
    unsigned long timeout = millis() + 100;
    while (!stream->available() && millis() < timeout);

    if (!stream->available()) return;                               // Timeout

    char c3 = stream->read();
    if (c3 == '~') {
        switch (keyCode) {
            case '2':                                           // Insert key
                handleInsertKey();
                break;
                
            case '3':                                           // Delete key
                handleDeleteKey();
                break;
                
            case '1':                                           // Home key (alternative)
            case '4':                                           // End key (alternative)
            case '5':                                           // Page Up
            case '6':                                           // Page Down
                // Could implement page up/down for command history scrolling
                break;
        }
    }
}

// Arrow key handlers
void Console::handleArrowUp() {
    String prevCommand = commandHistory.getPrevious();
    if (prevCommand.length() > 0) {
        clearAndRedrawLine(prevCommand);
    }
}

void Console::handleArrowDown() {
    String nextCommand = commandHistory.getNext();
    clearAndRedrawLine(nextCommand);
}

void Console::handleArrowRight() {
    if (cursorPos < (int)inputBuffer.length()) {
    PRINT("\033[C");
        cursorPos++;
    }
}

void Console::handleArrowLeft() {
    if (cursorPos > 0) {
    PRINT("\033[D");
        cursorPos--;
    }
}

// Navigation key handlers
void Console::handleHome() {
    while (cursorPos > 0) {
    PRINT("\033[D");
        cursorPos--;
    }
}

void Console::handleEnd() {
    while (cursorPos < (int)inputBuffer.length()) {
    PRINT("\033[C");
        cursorPos++;
    }
}

// Special key handlers
void Console::handleInsertKey() {
    insertMode = !insertMode;
    // Change cursor type based on mode
    PRINT(insertMode ? "\033[4 q" : "\033[2 q");           // Underline for insert, block for overwrite
}

void Console::handleDeleteKey() {
    if (cursorPos < (int)inputBuffer.length()) {
        inputBuffer.remove(cursorPos, 1);
        refreshLineFromCursor();
    }
}

void Console::handleBackspace() {
    if (cursorPos > 0) {
        cursorPos--;
        inputBuffer.remove(cursorPos, 1);
    PRINT("\b");
        refreshLineFromCursor();
    }
}

void Console::handleClearScreen() {
    PRINT("\033[2J\033[H");                                 // Clear screen and move cursor to home
    inputBuffer = "";
    cursorPos = 0;
    PRINT(shell);
}

void Console::handleNewline() {
    if (inputBuffer.length() > 0) {
        PRINT("\n\r");
        processInput(inputBuffer);
        resetInputState();
        PRINT(shell);
    } else {
        PRINT("\n\r");
        PRINT(shell);
    }
}

// Clear current input line and redraw with new content
void Console::clearAndRedrawLine(const String& newContent) {
    // Move cursor to beginning of input
    while (cursorPos > 0) {
    PRINT("\b");
        cursorPos--;
    }
    
    // Clear the entire line content
    PRINT("\033[K");                                        // Clear from cursor to end of line
    
    // Set new content and redraw
    inputBuffer = newContent;
    PRINT(inputBuffer);
    cursorPos = inputBuffer.length();
}

// Refresh line after cursor position changes
void Console::refreshLineFromCursor() {
    PRINT("\033[K");                                        // Clear from cursor to end
    String remaining = inputBuffer.substring(cursorPos);
    PRINT(remaining);

    // Move cursor back to correct position
    for (int i = 0; i < (int)remaining.length(); i++) {
    PRINT("\033[D");
    }
}

// Helper to reset input state after command processing
void Console::resetInputState() {
    inputBuffer = "";
    cursorPos = 0;
    commandHistory.resetToEnd();
    insertMode = true;
        PRINT("\033[4 q");                                      // Reset to insert mode cursor
}

//--------------------------------------------------------------------------------------------------------------------------------

// System commands (help, cls, status)
bool Console::runConsoleCommands(const String& cmd, const String& args) {
    if (cmd == "?" || cmd == "h") {
        printConsoleHelp();
        return true;
    
    } else if (cmd == "??") {
        printAllHelp();
        return true;

    } else if (cmd == "cls" || cmd == "clear") {
        PRINT("\033[2J\033[H");                     // ANSI escape code to clear screen and move cursor to home
        PRINT(shell);
        return true;

    } else if (cmd == "debug") {
        // Set debug level: debug 0-4
        if (args.length() > 0) {
            int level = args.toInt();
            if (level >= DEBUG_NON && level <= DEBUG_DBG) {
                log::setDebugLevel((DebugLevel)level);
                LOG_INF("Debug level set to: " + String(level));
            } else {
                LOG_ERR("Invalid debug level. Use 0-4 (0=NON, 1=ERR, 2=WRN, 3=INF, 4=DBG)");
            }

        } else {
            PRINTLN("Current debug level: " + String(log::getDebugLevel()));
        }
        return true;

    } else if (cmd == "color") {
        // Toggle color output: color on/off
        if (args == "on" || args == "1" || args == "true") {
            log::setColorEnabled(true);
        } else if (args == "off" || args == "0" || args == "false") {
            log::setColorEnabled(false);
        } else {
            PRINTLN("Current color setting: " + String(log::getColorEnabled() ? "enabled" : "disabled"));
        }
        return true;

    } else if (cmd == "test") {
        // Test all log levels
    PRINTLN("This is a normal message");
    LOG_ERR("This is an error message");
    LOG_WRN("This is a warning message");
    LOG_INF("This is an info message");
    LOG_DBG("This is a debug message");
    PRINTLN("Current debug level: " + String(log::getDebugLevel()));
    PRINTLN("Color output is " + String(log::getColorEnabled() ? "enabled" : "disabled"));
        return true;

    } 
    return false;
}

// Leg control commands (operates on leg 0 by default, could be extended for specific legs)
bool Console::runLegCommand(const String& cmd, const String& args) {
    int legIndex = 0; // Default to leg 0
    if (args.length() > 0) {
        legIndex = args.toInt();
        if (legIndex < 0 || legIndex >= HEXAPOD_LEGS) {
            legIndex = 0;
        }
    }
    
    // Call the specific leg's runConsoleCommands method
    return hexapod->legs[legIndex].runConsoleCommands(cmd, args, legIndex);
}

//Print comprehensive help information
bool Console::printAllHelp() {

    PRINTLN("SpiderBot Console - Available Commands:");
    PRINTLN("");
    
    // Print help for each component
    if (!this->printConsoleHelp()) return false;
    if (!mc->printConsoleHelp()) return false;
    if (!driver->printConsoleHelp()) return false;
    if (!servo->printConsoleHelp()) return false;
    if (!IK::printConsoleHelp()) return false;
    if (!hexapod->printConsoleHelp()) return false;
    if (!hexapod->legs[0].printConsoleHelp()) return false;
    if (!turret->printConsoleHelp()) return false;
    if (!sensor->printConsoleHelp()) return false;
    if (!bodyPose->printConsoleHelp()) return false;
    if (!gc->printConsoleHelp()) return false;
    if (!rc->printConsoleHelp()) return false;

    // Show examples for common commands
    PRINTLN("Examples: 'lpu 2' moves leg 2 point up, 'sbu 72 200' plays note, 'mlon 3' turns on user LED 3");

    return true;
}

// Print console-specific help information
bool Console::printConsoleHelp() {
    PRINTLN("Console Commands:\n\r");
    PRINTLN("  ? / h            - Show this help message");
    PRINTLN("  ??               - Show all available commands");
    PRINTLN("");
    PRINTLN("  d?               - Show driver commands");
    PRINTLN("  s?               - Show servo commands");
    PRINTLN("  k?               - Show kinematics commands");
    PRINTLN("  h?               - Show hexapod commands");
    PRINTLN("  l?               - Show leg commands");
    PRINTLN("  a?               - Show AX-S1 Sensor commands");
    PRINTLN("  t?               - Show turret commands");
    PRINTLN("  b?               - Show body pose commands");
    PRINTLN("  g?               - Show gait controller commands");
    PRINTLN("  r?               - Show remote controller commands");
    PRINTLN("");
    PRINTLN("  cls / clear      - Clear the terminal screen");
    PRINTLN("  debug [0-4]      - Set debug level (0=NONE, 1=ERROR, 2=WARN, 3=INFO, 4=ALL)");
    PRINTLN("  color [on/off]   - Enable/disable color output");
    PRINTLN("  test             - Test all log message types");
    PRINTLN("");
    return true;
}

