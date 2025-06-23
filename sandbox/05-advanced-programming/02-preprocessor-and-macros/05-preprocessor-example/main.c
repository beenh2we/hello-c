#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ====================================================
// Configuration section
// ====================================================

// Version information
#define VERSION_MAJOR 1
#define VERSION_MINOR 2
#define VERSION_PATCH 3

// Feature flags
#define ENABLE_LOGGING 1
#define ENABLE_EXTENDED_FEATURES 1
#define ENABLE_EXPERIMENTAL 0

// Debug settings
#ifdef NDEBUG
    #define DEBUG_MODE 0
#else
    #define DEBUG_MODE 1
#endif

// Platform-specific configuration
#if defined(_WIN32)
    #define PLATFORM "Windows"
    #define PATH_SEPARATOR "\\"
#elif defined(__APPLE__)
    #define PLATFORM "macOS"
    #define PATH_SEPARATOR "/"
#elif defined(__linux__)
    #define PLATFORM "Linux"
    #define PATH_SEPARATOR "/"
#else
    #define PLATFORM "Unknown"
    #define PATH_SEPARATOR "/"
#endif

// ====================================================
// Utility macros
// ====================================================

// String utilities
#define STR(x) #x
#define XSTR(x) STR(x)
#define CONCAT(a, b) a ## b

// Version string
#define VERSION_STR XSTR(VERSION_MAJOR) "." XSTR(VERSION_MINOR) "." XSTR(VERSION_PATCH)

// Max/Min macros
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Memory management
#define ALLOC(type, count) ((type*)malloc(sizeof(type) * (count)))
#define FREE(ptr) do { free(ptr); (ptr) = NULL; } while(0)

// Logging macros
#if ENABLE_LOGGING
    #define LOG_INFO(msg, ...) printf("[INFO] " msg "\n", ##__VA_ARGS__)
    #define LOG_ERROR(msg, ...) fprintf(stderr, "[ERROR] %s:%d: " msg "\n", \
                                __FILE__, __LINE__, ##__VA_ARGS__)
    #if DEBUG_MODE
        #define LOG_DEBUG(msg, ...) printf("[DEBUG] %s:%d: " msg "\n", \
                                   __FILE__, __LINE__, ##__VA_ARGS__)
    #else
        #define LOG_DEBUG(msg, ...) ((void)0)
    #endif
#else
    #define LOG_INFO(msg, ...) ((void)0)
    #define LOG_ERROR(msg, ...) ((void)0)
    #define LOG_DEBUG(msg, ...) ((void)0)
#endif

// Assertion macro
#if DEBUG_MODE
    #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                LOG_ERROR("Assertion failed: %s, Message: %s", #condition, message); \
                abort(); \
            } \
        } while(0)
#else
    #define ASSERT(condition, message) ((void)0)
#endif

// Feature-based code inclusion
#if ENABLE_EXTENDED_FEATURES
    #define EXTENDED_FEATURE(code) code
#else
    #define EXTENDED_FEATURE(code)
#endif

// Loop utilities
#define REPEAT(count) for(int _i = 0; _i < (count); _i++)

// ====================================================
// Application code
// ====================================================

// Simple data structure
typedef struct {
    char name[50];
    int value;
} Item;

// Function to create an item
Item* create_item(const char* name, int value) {
    LOG_DEBUG("Creating item: %s with value: %d", name, value);
    
    // Validate inputs
    ASSERT(name != NULL, "Item name cannot be NULL");
    ASSERT(strlen(name) < 50, "Item name too long");
    
    Item* item = ALLOC(Item, 1);
    if (!item) {
        LOG_ERROR("Memory allocation failed");
        return NULL;
    }
    
    strcpy(item->name, name);
    item->value = value;
    
    return item;
}

// Function to print version info
void print_version() {
    printf("Application Version: %s\n", VERSION_STR);
    printf("Platform: %s\n", PLATFORM);
    printf("Debug Mode: %s\n", DEBUG_MODE ? "Enabled" : "Disabled");
    
    #if ENABLE_EXPERIMENTAL
        printf("Experimental Features: Enabled\n");
    #else
        printf("Experimental Features: Disabled\n");
    #endif
}

// Extended feature demo
EXTENDED_FEATURE(
void advanced_feature() {
    LOG_INFO("Advanced feature running");
    printf("This code only compiles if ENABLE_EXTENDED_FEATURES is set\n");
}
)

int main() {
    printf("==== COMPREHENSIVE PREPROCESSOR EXAMPLE ====\n\n");
    
    LOG_INFO("Application starting");
    print_version();
    
    LOG_DEBUG("Debug information visible in debug builds");
    
    // Create some items
    Item* items[3];
    const char* names[] = {"Apple", "Banana", "Cherry"};
    
    for (int i = 0; i < 3; i++) {
        items[i] = create_item(names[i], (i+1) * 10);
        if (items[i]) {
            LOG_INFO("Created item %d: %s (value: %d)", i, items[i]->name, items[i]->value);
        }
    }
    
    // Show use of MAX/MIN macros
    printf("\nMaximum value: %d\n", MAX(items[0]->value, MAX(items[1]->value, items[2]->value)));
    
    // Use repeat macro
    printf("\nRepeating message:\n");
    REPEAT(3) {
        printf("Iteration %d\n", _i);
    }
    
    // Conditional feature
    #if ENABLE_EXTENDED_FEATURES
        printf("\nExtended features enabled\n");
        advanced_feature();
    #endif
    
    // Clean up
    for (int i = 0; i < 3; i++) {
        FREE(items[i]);
    }
    
    LOG_INFO("Application shutting down");
    return 0;
}