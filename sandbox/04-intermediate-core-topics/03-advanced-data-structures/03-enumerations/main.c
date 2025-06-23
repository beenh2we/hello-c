#include <stdio.h>

// Basic enumeration
enum Days {
    SUNDAY,    // 0
    MONDAY,    // 1
    TUESDAY,   // 2
    WEDNESDAY, // 3
    THURSDAY,  // 4
    FRIDAY,    // 5
    SATURDAY   // 6
};

// Enumeration with specific values
enum Colors {
    RED = 0xFF0000,
    GREEN = 0x00FF00,
    BLUE = 0x0000FF,
    YELLOW = 0xFFFF00,
    CYAN = 0x00FFFF,
    MAGENTA = 0xFF00FF,
    WHITE = 0xFFFFFF,
    BLACK = 0x000000
};

// Enumeration for status codes
enum StatusCode {
    SUCCESS = 0,
    ERROR_FILE_NOT_FOUND = -1,
    ERROR_PERMISSION_DENIED = -2,
    ERROR_OUT_OF_MEMORY = -3,
    ERROR_INVALID_ARGUMENT = -100
};

// Enum used as a "namespace"
enum MathConstants {
    MATH_PI = 314159,     // 3.14159 (scaled by 100000)
    MATH_E = 271828,      // 2.71828 (scaled by 100000)
    MATH_PHI = 161803     // 1.61803 (scaled by 100000)
};

// Enumeration for state machine
enum State {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_PAUSED,
    STATE_STOPPED,
    STATE_ERROR
};

// Forward state transitions
enum StateTransition {
    TRANSITION_START,
    TRANSITION_STOP,
    TRANSITION_PAUSE,
    TRANSITION_RESUME,
    TRANSITION_RESET,
    TRANSITION_ERROR
};

// Function to convert state enums to strings
const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "Idle";
        case STATE_RUNNING: return "Running";
        case STATE_PAUSED: return "Paused";
        case STATE_STOPPED: return "Stopped";
        case STATE_ERROR: return "Error";
        default: return "Unknown";
    }
}

// Function to handle state transitions
enum State transition_state(enum State current_state, enum StateTransition transition) {
    switch (current_state) {
        case STATE_IDLE:
            if (transition == TRANSITION_START) return STATE_RUNNING;
            if (transition == TRANSITION_ERROR) return STATE_ERROR;
            break;
        case STATE_RUNNING:
            if (transition == TRANSITION_PAUSE) return STATE_PAUSED;
            if (transition == TRANSITION_STOP) return STATE_STOPPED;
            if (transition == TRANSITION_ERROR) return STATE_ERROR;
            break;
        case STATE_PAUSED:
            if (transition == TRANSITION_RESUME) return STATE_RUNNING;
            if (transition == TRANSITION_STOP) return STATE_STOPPED;
            if (transition == TRANSITION_ERROR) return STATE_ERROR;
            break;
        case STATE_STOPPED:
            if (transition == TRANSITION_RESET) return STATE_IDLE;
            if (transition == TRANSITION_ERROR) return STATE_ERROR;
            break;
        case STATE_ERROR:
            if (transition == TRANSITION_RESET) return STATE_IDLE;
            break;
    }
    
    // No valid transition, return current state
    return current_state;
}

int main() {
    printf("==== ENUMERATION EXAMPLES ====\n\n");
    
    // 1. Basic Enumeration Example
    printf("--- Basic Enumeration Example ---\n");
    
    enum Days today = WEDNESDAY;
    
    printf("Today is day %d of the week\n", today);
    
    if (today == WEDNESDAY) {
        printf("It's Wednesday!\n");
    }
    
    printf("Days of the week:\n");
    printf("SUNDAY = %d\n", SUNDAY);
    printf("MONDAY = %d\n", MONDAY);
    printf("TUESDAY = %d\n", TUESDAY);
    printf("WEDNESDAY = %d\n", WEDNESDAY);
    printf("THURSDAY = %d\n", THURSDAY);
    printf("FRIDAY = %d\n", FRIDAY);
    printf("SATURDAY = %d\n\n", SATURDAY);
    
    // 2. Enumeration with Specific Values
    printf("--- Enumeration with Specific Values ---\n");
    
    enum Colors background = WHITE;
    enum Colors foreground = BLUE;
    
    printf("Background color: 0x%06X\n", background);
    printf("Foreground color: 0x%06X\n\n", foreground);
    
    // 3. Status Code Enumeration
    printf("--- Status Code Enumeration ---\n");
    
    enum StatusCode status = ERROR_PERMISSION_DENIED;
    
    printf("Operation status: %d\n", status);
    
    // Using enum for control flow
    switch (status) {
        case SUCCESS:
            printf("Operation succeeded\n");
            break;
        case ERROR_FILE_NOT_FOUND:
            printf("Error: File not found\n");
            break;
        case ERROR_PERMISSION_DENIED:
            printf("Error: Permission denied\n");
            break;
        case ERROR_OUT_OF_MEMORY:
            printf("Error: Out of memory\n");
            break;
        case ERROR_INVALID_ARGUMENT:
            printf("Error: Invalid argument\n");
            break;
        default:
            printf("Unknown error\n");
    }
    printf("\n");
    
    // 4. Math Constants Example
    printf("--- Math Constants Example ---\n");
    
    double pi = MATH_PI / 100000.0;
    double e = MATH_E / 100000.0;
    double phi = MATH_PHI / 100000.0;
    
    printf("π = %.5f\n", pi);
    printf("e = %.5f\n", e);
    printf("φ = %.5f\n\n", phi);
    
    // 5. State Machine Example
    printf("--- State Machine Example ---\n");
    
    enum State current = STATE_IDLE;
    printf("Initial state: %s\n", state_to_string(current));
    
    // Simulate state transitions
    current = transition_state(current, TRANSITION_START);
    printf("After START: %s\n", state_to_string(current));
    
    current = transition_state(current, TRANSITION_PAUSE);
    printf("After PAUSE: %s\n", state_to_string(current));
    
    current = transition_state(current, TRANSITION_RESUME);
    printf("After RESUME: %s\n", state_to_string(current));
    
    current = transition_state(current, TRANSITION_ERROR);
    printf("After ERROR: %s\n", state_to_string(current));
    
    current = transition_state(current, TRANSITION_RESET);
    printf("After RESET: %s\n", state_to_string(current));
    
    // Enum size
    printf("\nSize of enum Days: %zu bytes\n", sizeof(enum Days));
    printf("Size of enum Colors: %zu bytes\n", sizeof(enum Colors));
    printf("Size of enum StatusCode: %zu bytes\n", sizeof(enum StatusCode));
    
    return 0;
}