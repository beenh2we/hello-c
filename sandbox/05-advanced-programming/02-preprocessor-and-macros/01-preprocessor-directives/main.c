#include <stdio.h>
#include <stdlib.h>

// Basic constant definition
#define PI           3.14159265358979323846
#define MAX_ELEMENTS 100
#define VERSION      "1.0.2"
#define DEBUG_MODE

// String concatenation with ##
#define CONCAT(a, b) a##b

// Stringification with #
#define STRINGIFY(x) #x
#define TOSTRING(x)  STRINGIFY(x)

// Multiline macro
#define ERROR_MESSAGE(msg)                                       \
    fprintf(stderr, "Error: %s\n", msg);                         \
    fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__); \
    exit(EXIT_FAILURE)

// Function to demonstrate constants
void demo_constants()
{
    printf("=== Constants defined with #define ===\n");
    printf("PI = %f\n", PI);
    printf("MAX_ELEMENTS = %d\n", MAX_ELEMENTS);
    printf("VERSION = %s\n", VERSION);

    // Demonstrates that #define is text substitution
    printf("PI * 2 = %f\n", PI * 2);

    int array[MAX_ELEMENTS];
    printf("Size of array = %zu bytes\n", sizeof(array));
}

// Function to demonstrate directive operators (# and ##)
void demo_directive_operators()
{
    printf("\n=== Directive operators (# and ##) ===\n");

    // ## operator for token concatenation
    int value12 = 42;
    printf("CONCAT(value, 12) = %d\n", CONCAT(value, 12));

    // # operator for stringification
    printf("STRINGIFY(Hello World) = %s\n", STRINGIFY(Hello World));
    printf("TOSTRING(MAX_ELEMENTS) = %s\n", TOSTRING(MAX_ELEMENTS));
    printf("STRINGIFY(MAX_ELEMENTS) = %s\n", STRINGIFY(MAX_ELEMENTS));

    // Can be useful for printing variable names
    int x = 10;
    printf("%s = %d\n", STRINGIFY(x), x);
}

// Function to demonstrate #undef
void demo_undef()
{
    printf("\n=== #undef directive ===\n");

#define TEMP_VALUE 100
    printf("TEMP_VALUE = %d\n", TEMP_VALUE);

// Undefine TEMP_VALUE
#undef TEMP_VALUE

// Redefine with a new value
#define TEMP_VALUE 200
    printf("TEMP_VALUE redefined = %d\n", TEMP_VALUE);
}

int main()
{
    printf("==== PREPROCESSOR DIRECTIVES ====\n\n");

    demo_constants();
    demo_directive_operators();
    demo_undef();

    printf(
        "\nIf you uncomment the ERROR_MESSAGE line below, the program will terminate:\n");
    // ERROR_MESSAGE("Demonstrating error handling macro");

    return 0;
}
