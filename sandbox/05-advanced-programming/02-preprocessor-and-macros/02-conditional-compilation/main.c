#include <stdio.h>

// Define some configuration constants
#define WINDOWS_BUILD 0
#define LINUX_BUILD 1
#define MAC_BUILD 0

#define FEATURE_NETWORKING 1
#define FEATURE_GRAPHICS 1
#define FEATURE_AUDIO 0

#define DEBUG_LEVEL 2  // 0=none, 1=basic, 2=verbose

// Example of system-specific code using #if, #elif, #else
void demo_platform_specific() {
    printf("=== Platform-specific code ===\n");
    
    #if WINDOWS_BUILD
        printf("Windows-specific code compiled\n");
        // Windows-specific declarations would go here
    #elif LINUX_BUILD
        printf("Linux-specific code compiled\n");
        // Linux-specific declarations would go here
    #elif MAC_BUILD
        printf("macOS-specific code compiled\n");
        // macOS-specific declarations would go here
    #else
        printf("Unknown platform\n");
        // Default/generic declarations would go here
    #endif
    
    // Sometimes checking for specific features is better than
    // checking for specific platforms
    printf("\nCompiled with:\n");
    
    #if FEATURE_NETWORKING
        printf("- Networking support\n");
    #endif
    
    #if FEATURE_GRAPHICS
        printf("- Graphics support\n");
    #endif
    
    #if FEATURE_AUDIO
        printf("- Audio support\n");
    #endif
}

// Using #ifdef, #ifndef for simpler cases
void demo_ifdef_ifndef() {
    printf("\n=== #ifdef and #ifndef ===\n");
    
    // Check if DEBUG_MODE is defined (from previous example)
    #ifdef DEBUG_MODE
        printf("DEBUG_MODE is defined\n");
    #else
        printf("DEBUG_MODE is not defined\n");
    #endif
    
    // Same as #ifdef, just inverted logic
    #ifndef RELEASE_MODE
        printf("RELEASE_MODE is not defined\n");
    #else
        printf("RELEASE_MODE is defined\n");
    #endif
    
    // Note: #ifdef X is equivalent to #if defined(X)
    // and #ifndef X is equivalent to #if !defined(X)
}

// Using the defined() operator
void demo_defined_operator() {
    printf("\n=== defined() operator ===\n");
    
    #if defined(DEBUG_MODE) && !defined(RELEASE_MODE)
        printf("DEBUG build confirmed\n");
    #endif
    
    #if defined(WINDOWS_BUILD) && WINDOWS_BUILD == 1
        printf("Windows build confirmed\n");
    #elif defined(LINUX_BUILD) && LINUX_BUILD == 1
        printf("Linux build confirmed\n");
    #endif
}

// Using conditional compilation for debugging
void demo_debug_levels() {
    printf("\n=== Debugging levels using #if ===\n");
    
    int value = 42;
    
    printf("Current DEBUG_LEVEL: %d\n", DEBUG_LEVEL);
    
    // Basic tracing always included if any debug is enabled
    #if DEBUG_LEVEL >= 1
        printf("DEBUG[1]: Function demo_debug_levels() called\n");
        printf("DEBUG[1]: value = %d\n", value);
    #endif
    
    // More verbose debugging
    #if DEBUG_LEVEL >= 2
        printf("DEBUG[2]: Additional internal details...\n");
        printf("DEBUG[2]: &value = %p\n", (void*)&value);
    #endif
    
    // This code will always be compiled (not conditional)
    printf("Function execution completed\n");
}

// Conditional compilation to handle compiler differences
void demo_compiler_differences() {
    printf("\n=== Compiler-specific code ===\n");
    
    #ifdef _MSC_VER
        printf("Microsoft Visual C++ detected, version: %d\n", _MSC_VER);
        // MSVC specific code would go here
    #elif defined(__GNUC__)
        printf("GCC detected, version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
        // GCC specific code would go here
    #elif defined(__clang__)
        printf("Clang detected\n");
        // Clang specific code would go here
    #else
        printf("Unknown compiler\n");
        // Generic code would go here
    #endif
}

int main() {
    printf("==== CONDITIONAL COMPILATION ====\n\n");
    
    demo_platform_specific();
    demo_ifdef_ifndef();
    demo_defined_operator();
    demo_debug_levels();
    demo_compiler_differences();
    
    printf("\nExample of a multi-line comment using conditional compilation:\n");
    
    #if 0
    This entire block is treated as a comment.
    It can contain anything, including code, special characters, even
    unmatched quotes " or unmatched comment markers /* like this.
    
    Very useful for temporarily disabling large blocks of code.
    #endif
    
    printf("That multi-line comment block was not compiled\n");
    
    return 0;
}