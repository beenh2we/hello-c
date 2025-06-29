#include <errno.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define error levels
typedef enum
{
    ERROR_NONE = 0,
    ERROR_WARNING,
    ERROR_ERROR,
    ERROR_FATAL
} ErrorLevel;

// Global error state
typedef struct
{
    int code;           // Error code (e.g., errno)
    ErrorLevel level;   // Error severity
    char message[256];  // Error message
    char function[64];  // Function where error occurred
    int line;           // Line number where error occurred
    time_t timestamp;   // When the error occurred
    int has_error;      // Whether an error is set
} ErrorState;

// Global error state
static ErrorState g_error_state = {0};

// Error logger function pointer
typedef void (*ErrorLoggerFunc)(const ErrorState *error);
static ErrorLoggerFunc g_error_logger = NULL;

// Jump buffer for error recovery
jmp_buf error_jmp_buf;
int error_jmp_active = 0;

// Function to clear the error state
void error_clear()
{
    memset(&g_error_state, 0, sizeof(ErrorState));
}

// Function to set an error with variable arguments
void error_set(ErrorLevel level,
               int code,
               const char *function,
               int line,
               const char *format,
               ...)
{
    g_error_state.code = code;
    g_error_state.level = level;
    g_error_state.timestamp = time(NULL);
    g_error_state.has_error = 1;
    strncpy(
        g_error_state.function, function, sizeof(g_error_state.function) - 1);
    g_error_state.line = line;

    // Format the error message
    va_list args;
    va_start(args, format);
    vsnprintf(
        g_error_state.message, sizeof(g_error_state.message), format, args);
    va_end(args);

    // Call the error logger if registered
    if (g_error_logger != NULL)
    {
        g_error_logger(&g_error_state);
    }

    // If this is a fatal error and error recovery is active, longjmp
    if (level == ERROR_FATAL && error_jmp_active)
    {
        longjmp(error_jmp_buf, 1);
    }
}

// Macro to make it easier to set errors with current function and line
#define SET_ERROR(level, code, format, ...) \
    error_set(level, code, __func__, __LINE__, format, ##__VA_ARGS__)

// Standard error logger that prints to stderr
void error_logger_stderr(const ErrorState *error)
{
    const char *level_str;
    switch (error->level)
    {
    case ERROR_WARNING:
        level_str = "WARNING";
        break;
    case ERROR_ERROR:
        level_str = "ERROR";
        break;
    case ERROR_FATAL:
        level_str = "FATAL";
        break;
    default:
        level_str = "UNKNOWN";
        break;
    }

    fprintf(stderr,
            "[%s] %s (in %s:%d): %s\n",
            level_str,
            error->code ? strerror(error->code) : "Application error",
            error->function,
            error->line,
            error->message);
}

// Function to register an error logger
void error_register_logger(ErrorLoggerFunc logger)
{
    g_error_logger = logger;
}

// Safe file opening function with error handling
FILE *safe_fopen(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        SET_ERROR(ERROR_ERROR,
                  errno,
                  "Failed to open file '%s' with mode '%s'",
                  filename,
                  mode);
    }
    return file;
}

// Function that demonstrates error recovery using setjmp/longjmp
void demonstrate_error_recovery()
{
    printf("\n=== Error Recovery Demo ===\n");

    // Set up error recovery point
    error_jmp_active = 1;

    if (setjmp(error_jmp_buf) == 0)
    {
        // Normal execution path
        printf("Starting operations that might cause a fatal error...\n");

        // Simulate a fatal error
        SET_ERROR(ERROR_FATAL, 0, "A simulated fatal error occurred!");

        // This shouldn't be reached
        printf("This line should not be printed\n");
    }
    else
    {
        // Recovery path after longjmp
        printf("Recovered from fatal error: %s\n", g_error_state.message);
        error_clear();
    }

    error_jmp_active = 0;
    printf("Continuing after error recovery\n");
}

// Function to demonstrate a complete file handling operation with error handling
void write_file_with_error_handling()
{
    FILE *file = NULL;
    char *buffer = NULL;
    int success = 0;

    // Allocate a buffer
    buffer = malloc(1024);
    if (buffer == NULL)
    {
        SET_ERROR(ERROR_FATAL, errno, "Failed to allocate memory");
        goto cleanup;
    }

    // Fill the buffer with test data
    memset(buffer, 'A', 1024);

    // Open the file
    file = safe_fopen("test_error_handling.txt", "w");
    if (file == NULL)
    {
        // Error already set by safe_fopen
        goto cleanup;
    }

    // Write to the file
    size_t written = fwrite(buffer, 1, 1024, file);
    if (written != 1024)
    {
        SET_ERROR(ERROR_ERROR, errno, "Failed to write all data");
        goto cleanup;
    }

    // Success!
    success = 1;

cleanup:
    // Clean up resources
    if (file != NULL)
    {
        fclose(file);
    }

    if (buffer != NULL)
    {
        free(buffer);
    }

    if (success)
    {
        printf("File written successfully\n");
    }
    else
    {
        printf("Failed to write file: %s\n", g_error_state.message);
        error_clear();
    }
}

int main()
{
    printf("=== Advanced Error Handling Demo ===\n");

    // Register the error logger
    error_register_logger(error_logger_stderr);

    // Demonstrate different error levels
    SET_ERROR(ERROR_WARNING, 0, "This is a warning message");

    // Demonstrate error recovery with longjmp
    demonstrate_error_recovery();

    // Demonstrate complete file handling with cleanup
    write_file_with_error_handling();

    // Clean up
    unlink("test_error_handling.txt");

    return 0;
}
