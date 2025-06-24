#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple macro function
#define SQUARE(x) ((x) * (x))

// Macro with multiple arguments
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Common error - macro without proper parentheses
#define BAD_SQUARE(x) x *x

// Macro for safe array indexing
#define ARRAY_INDEX(arr, i) \
    ((i) < 0 || (i) >= sizeof(arr) / sizeof(arr[0]) ? (printf("Array index %d out of bounds\n", (i)), 0) : (arr)[(i)])

// Macro to create a dynamic string
#define MAKE_STR(size) ((char *) malloc((size) + 1))

// Macro to safely free memory
#define SAFE_FREE(ptr)     \
    do                     \
    {                      \
        if ((ptr) != NULL) \
        {                  \
            free(ptr);     \
            (ptr) = NULL;  \
        }                  \
    } while (0)

// Macro for debugging
#define DEBUG_PRINT(format, ...) printf("[DEBUG] %s:%d: " format "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Generic macro to validate a condition
#define VALIDATE(condition, message)                                               \
    do                                                                             \
    {                                                                              \
        if (!(condition))                                                          \
        {                                                                          \
            fprintf(stderr, "Error at %s:%d - %s\n", __FILE__, __LINE__, message); \
            exit(EXIT_FAILURE);                                                    \
        }                                                                          \
    } while (0)

// Macro to trace function entry and exit
#define TRACE_ENTER() printf("Entering %s\n", __func__)
#define TRACE_EXIT()  printf("Exiting %s\n", __func__)

// Demonstration of simple macro functions
void demo_simple_macros()
{
    printf("=== Simple Macro Functions ===\n");

    int a = 5;
    printf("SQUARE(5) = %d\n", SQUARE(5));
    printf("SQUARE(a) = %d\n", SQUARE(a));

    // This shows what happens when macros don't have parentheses
    printf("SQUARE(a+1) = %d\n", SQUARE(a + 1));          // ((a+1) * (a+1))
    printf("BAD_SQUARE(a+1) = %d\n", BAD_SQUARE(a + 1));  // a+1 * a+1 = a + (1*a) + 1 = 2a + 1

    // Multiple argument macros
    printf("MAX(10, 5) = %d\n", MAX(10, 5));
    printf("MIN(10, 5) = %d\n", MIN(10, 5));

    // Demonstrating side effects
    printf("\nBeware of side effects in macros:\n");
    int i = 5;
    printf("i = %d\n", i);
    printf("SQUARE(i++) = %d\n", SQUARE(i++));  // i++ is evaluated twice!
    printf("After macro, i = %d\n", i);         // i is incremented twice
}

// Demonstrate advanced macro usage
void demo_advanced_macros()
{
    printf("\n=== Advanced Macro Functions ===\n");

    // Array bounds checking
    int numbers[] = {10, 20, 30, 40, 50};
    int valid_index = 2;
    int invalid_index = 10;

    printf("Safe array access: ARRAY_INDEX(numbers, %d) = %d\n", valid_index, ARRAY_INDEX(numbers, valid_index));

    printf("Attempting invalid index %d: ", invalid_index);
    int result = ARRAY_INDEX(numbers, invalid_index);
    printf("Returned %d\n", result);

    // Dynamic string allocation
    char *name = MAKE_STR(50);
    VALIDATE(name != NULL, "Memory allocation failed");

    strcpy(name, "Hello, macro world!");
    printf("Dynamic string: %s\n", name);

    // Safe memory freeing
    printf("Freeing memory and setting pointer to NULL\n");
    SAFE_FREE(name);
    printf("After SAFE_FREE, name is %p\n", (void *) name);

    // Debug output
    DEBUG_PRINT("This is a debug message");
    DEBUG_PRINT("Value = %d", 42);
}

// Function to show the proper use of do-while(0) in macros
void demo_do_while_macros()
{
    printf("\n=== Using do-while(0) in Macros ===\n");

    printf("Benefits of do-while(0) in macros:\n");
    printf("1. Allows use of multiple statements in if-else blocks\n");
    printf("2. Allows semicolon after macro invocation\n");
    printf("3. Creates a compound statement with its own scope\n");
    printf("4. Protects against macro expansion issues\n");

// Example without do-while
#define BAD_MACRO(x)          \
    printf("Value: %d\n", x); \
    x++

    if (1)
        BAD_MACRO(10);  // Works fine here
    else
        printf("This would cause syntax error if reached\n");

// Example with do-while
#define GOOD_MACRO(x)             \
    do                            \
    {                             \
        printf("Value: %d\n", x); \
        x++;                      \
    } while (0)

    printf("\nWith do-while(0):\n");
    int val = 20;
    if (1)
        GOOD_MACRO(val);  // Works properly
    else
        printf("No syntax issues\n");

    printf("After GOOD_MACRO, val = %d\n", val);
}

// Function to demonstrate function-like macros vs. inline functions
void demo_macro_vs_function()
{
    printf("\n=== Macro vs. Function Comparison ===\n");

#define MACRO_ABS(x) ((x) < 0 ? -(x) : (x))

    // Equivalent inline function
    inline int func_abs(int x)
    {
        return x < 0 ? -x : x;
    }

    int val1 = -5;
    int val2 = 10;

    printf("MACRO_ABS(%d) = %d\n", val1, MACRO_ABS(val1));
    printf("MACRO_ABS(%d) = %d\n", val2, MACRO_ABS(val2));

    printf("func_abs(%d) = %d\n", val1, func_abs(val1));
    printf("func_abs(%d) = %d\n", val2, func_abs(val2));

    printf("\nKey differences:\n");
    printf("1. Macros perform text substitution, functions call a separate code block\n");
    printf("2. Macros have no type checking, functions do\n");
    printf("3. Macros can have subtle evaluation bugs (side effects, etc.)\n");
    printf("4. Functions have call overhead, macros don't\n");
    printf("5. Macros can work with any type, functions need specific types\n");
}

// Function to trace with macros
void function_with_tracing()
{
    TRACE_ENTER();
    printf("Doing some work...\n");
    TRACE_EXIT();
}

int main()
{
    printf("==== MACRO FUNCTIONS ====\n\n");

    demo_simple_macros();
    demo_advanced_macros();
    demo_do_while_macros();
    demo_macro_vs_function();

    printf("\n=== Function Tracing with Macros ===\n");
    function_with_tracing();

    return 0;
}
