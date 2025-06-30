#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to demonstrate standard predefined macros
void demo_standard_macros()
{
    printf("=== Standard Predefined Macros ===\n");

    printf("__FILE__: %s\n", __FILE__);  // Current file path
    printf("__LINE__: %d\n", __LINE__);  // Current line number
    printf("__func__: %s\n", __func__);  // Current function name (C99)
    printf("__DATE__: %s\n", __DATE__);  // Compilation date MM DD YYYY
    printf("__TIME__: %s\n", __TIME__);  // Compilation time HH:MM:SS

    // C standard versions
    printf("__STDC__: %d\n", __STDC__);  // 1 if compiler conforms to ISO C

#ifdef __STDC_VERSION__
    printf("__STDC_VERSION__: %ldL (", __STDC_VERSION__);
    // Interpreting the C standard version
    if (__STDC_VERSION__ >= 201710L)
        printf("C17/C18 or later");
    else if (__STDC_VERSION__ >= 201112L)
        printf("C11");
    else if (__STDC_VERSION__ >= 199901L)
        printf("C99");
    else
        printf("Unknown C standard");
    printf(")\n");
#else
    printf("__STDC_VERSION__ not defined (pre-C99)\n");
#endif

// Check for preprocessor features
#ifdef __STDC_HOSTED__
    printf("__STDC_HOSTED__: %d (hosted environment)\n", __STDC_HOSTED__);
#endif
}

// Function to demonstrate compiler-specific predefined macros
void demo_compiler_macros()
{
    printf("\n=== Compiler-Specific Predefined Macros ===\n");

// Microsoft Visual C++
#ifdef _MSC_VER
    printf("Microsoft Visual C++ detected\n");
    printf("_MSC_VER: %d\n", _MSC_VER);
    printf("_MSC_FULL_VER: %d\n", _MSC_FULL_VER);
#endif

// GCC
#ifdef __GNUC__
    printf("GCC detected\n");
    printf("__GNUC__: %d\n", __GNUC__);
    printf("__GNUC_MINOR__: %d\n", __GNUC_MINOR__);
    printf("__GNUC_PATCHLEVEL__: %d\n", __GNUC_PATCHLEVEL__);

#ifdef __GNUC_PATCHLEVEL__
    printf("GCC version: %d.%d.%d\n",
           __GNUC__,
           __GNUC_MINOR__,
           __GNUC_PATCHLEVEL__);
#else
    printf("GCC version: %d.%d\n", __GNUC__, __GNUC_MINOR__);
#endif
#endif

// Clang
#ifdef __clang__
    printf("Clang detected\n");
    printf("__clang_major__: %d\n", __clang_major__);
    printf("__clang_minor__: %d\n", __clang_minor__);
    printf("__clang_patchlevel__: %d\n", __clang_patchlevel__);
#endif

// Intel C++ Compiler
#ifdef __INTEL_COMPILER
    printf("Intel C++ Compiler detected\n");
    printf("__INTEL_COMPILER: %d\n", __INTEL_COMPILER);
#endif
}

// Function to demonstrate platform-specific predefined macros
void demo_platform_macros()
{
    printf("\n=== Platform-Specific Predefined Macros ===\n");

// Operating system detection
#ifdef _WIN32
    printf("Windows platform detected (_WIN32 is defined)\n");
#ifdef _WIN64
    printf("64-bit Windows platform (_WIN64 is defined)\n");
#else
    printf("32-bit Windows platform (_WIN64 is not defined)\n");
#endif
#endif

#ifdef __linux__
    printf("Linux platform detected (__linux__ is defined)\n");
#endif

#ifdef __APPLE__
    printf("Apple platform detected (__APPLE__ is defined)\n");
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    printf("iOS simulator\n");
#elif TARGET_OS_IPHONE
    printf("iOS device\n");
#elif TARGET_OS_MAC
    printf("Mac OS X\n");
#else
    printf("Other Apple platform\n");
#endif
#endif

#ifdef __unix__
    printf("Unix platform detected (__unix__ is defined)\n");
#endif

// Architecture detection
#ifdef __x86_64__
    printf("64-bit x86 architecture (__x86_64__ is defined)\n");
#endif

#ifdef __i386__
    printf("32-bit x86 architecture (__i386__ is defined)\n");
#endif

#ifdef __ARM_ARCH
    printf("ARM architecture (__ARM_ARCH is defined): %d\n", __ARM_ARCH);
#endif

#ifdef __aarch64__
    printf("64-bit ARM architecture (__aarch64__ is defined)\n");
#endif
}

// Creating a custom debug log macro using predefined macros
#define LOG_DEBUG(format, ...)                  \
    fprintf(stderr,                             \
            "[DEBUG] %s:%d:%s(): " format "\n", \
            __FILE__,                           \
            __LINE__,                           \
            __func__,                           \
            ##__VA_ARGS__)

#define LOG_INFO(format, ...)                  \
    fprintf(stderr,                            \
            "[INFO] %s:%d:%s(): " format "\n", \
            __FILE__,                          \
            __LINE__,                          \
            __func__,                          \
            ##__VA_ARGS__)

#define LOG_ERROR(format, ...)                  \
    fprintf(stderr,                             \
            "[ERROR] %s:%d:%s(): " format "\n", \
            __FILE__,                           \
            __LINE__,                           \
            __func__,                           \
            ##__VA_ARGS__)

// Function to demonstrate logging macros
void demo_logging_macros()
{
    printf("\n=== Logging with Predefined Macros ===\n");
    printf("Check stderr output for log messages\n");

    LOG_DEBUG("Debug message example");
    LOG_INFO("Info message with a value: %d", 42);
    LOG_ERROR("Error message with two values: %d and %s", 404, "Not Found");
}

// Macro to check if code is being compiled in debug or release mode
#if defined(DEBUG) || !defined(NDEBUG)
#define IS_DEBUG_BUILD 1
#else
#define IS_DEBUG_BUILD 0
#endif

// Function to check build configuration
void demo_build_config()
{
    printf("\n=== Build Configuration Detection ===\n");

    if (IS_DEBUG_BUILD)
    {
        printf("Debug build detected\n");
    }
    else
    {
        printf("Release build detected\n");
    }

// Check for assertions
#ifdef NDEBUG
    printf("Assertions are disabled (NDEBUG is defined)\n");
#else
    printf("Assertions are enabled (NDEBUG is not defined)\n");
#endif
}

// Create a dummy line-number based assertion macro
#define ASSERT_LINE(condition) \
    ((condition)               \
         ? (void) 0            \
         : printf("Assertion failed at line %d: %s\n", __LINE__, #condition))

int main()
{
    printf("==== PREDEFINED MACROS ====\n\n");

    demo_standard_macros();
    demo_compiler_macros();
    demo_platform_macros();
    demo_logging_macros();
    demo_build_config();

    printf("\n=== Using __LINE__ for Error Tracking ===\n");
    int x = 5;
    ASSERT_LINE(x == 10);  // This will print an assertion failure
    ASSERT_LINE(x == 5);   // This will succeed silently

    return 0;
}
