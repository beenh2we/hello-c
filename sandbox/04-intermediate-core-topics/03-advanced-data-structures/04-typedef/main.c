#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Basic typedef for primitive types
typedef int Integer;
typedef unsigned int UInteger;
typedef char Character;
typedef float Real;

// 2. Typedef for arrays
typedef int IntArray[10];
typedef char String[50];

// 3. Typedef for structures
typedef struct
{
    char name[50];
    int age;
    float height;
} Person;

// 4. Typedef for nested structures
typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    Point top_left;
    Point bottom_right;
} Rectangle;

// 5. Typedef for unions
typedef union
{
    int i;
    float f;
    char c;
} NumberVariant;

// 6. Typedef for enums
typedef enum
{
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

// 7. Typedef for function pointers
typedef int (*MathOperation)(int, int);
typedef void (*Callback)(void);
typedef void (*Logger)(const char*, ...);

// Function prototypes for math operations
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

// Simple logging function
void log_message(const char* format, ...);

// 8. Complex typedef examples

// Pointer to function returning pointer to array of pointers to characters
typedef char* (*(*ComplexFunction)(void) )[10];

// Array of function pointers
typedef int (*FunctionArray[4])(int, int);

// Function that takes function pointer as argument
void process_operation(MathOperation operation, int a, int b);

// Function implementations
int add(int a, int b)
{
    return a + b;
}
int subtract(int a, int b)
{
    return a - b;
}
int multiply(int a, int b)
{
    return a * b;
}
int divide(int a, int b)
{
    return b != 0 ? a / b : 0;
}

void log_message(const char* format, ...)
{
    printf("[LOG] %s\n", format);
}

void process_operation(MathOperation operation, int a, int b)
{
    printf("Result: %d\n", operation(a, b));
}

int main()
{
    printf("==== TYPEDEF EXAMPLES ====\n\n");

    // 1. Basic typedef usage
    printf("--- Basic Typedef Usage ---\n");

    Integer i = 42;
    UInteger ui = 100;
    Character c = 'A';
    Real r = 3.14f;

    printf("Integer: %d\n", i);
    printf("UInteger: %u\n", ui);
    printf("Character: %c\n", c);
    printf("Real: %f\n\n", r);

    // 2. Array typedefs
    printf("--- Array Typedefs ---\n");

    IntArray numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    String name = "John Doe";

    printf("IntArray: ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    printf("String: %s\n\n", name);

    // 3. Structure typedef
    printf("--- Structure Typedef ---\n");

    Person person = {"Alice Smith", 30, 5.8f};
    Person* p_person = &person;

    printf("Person: %s, %d years old, %.1f feet tall\n", person.name, person.age, person.height);
    printf("Via pointer: %s, %d years old, %.1f feet tall\n\n",
           p_person->name,
           p_person->age,
           p_person->height);

    // 4. Nested structure typedef
    printf("--- Nested Structure Typedef ---\n");

    Rectangle rect = {{10, 20}, {30, 40}};

    printf("Rectangle: (%d,%d) to (%d,%d)\n",
           rect.top_left.x,
           rect.top_left.y,
           rect.bottom_right.x,
           rect.bottom_right.y);

    int width = rect.bottom_right.x - rect.top_left.x;
    int height = rect.bottom_right.y - rect.top_left.y;
    printf("Width: %d, Height: %d, Area: %d\n\n", width, height, width * height);

    // 5. Union typedef
    printf("--- Union Typedef ---\n");

    NumberVariant num;
    num.i = 42;
    printf("As integer: %d\n", num.i);

    num.f = 3.14f;
    printf("As float: %f\n", num.f);

    num.c = 'X';
    printf("As character: %c\n\n", num.c);

    // 6. Enum typedef
    printf("--- Enum Typedef ---\n");

    Direction dir = EAST;

    printf("Direction value: %d\n", dir);

    switch (dir)
    {
    case NORTH:
        printf("Heading North\n");
        break;
    case EAST:
        printf("Heading East\n");
        break;
    case SOUTH:
        printf("Heading South\n");
        break;
    case WEST:
        printf("Heading West\n");
        break;
    }
    printf("\n");

    // 7. Function pointer typedef
    printf("--- Function Pointer Typedef ---\n");

    MathOperation op = add;
    printf("Add: %d\n", op(5, 3));

    op = subtract;
    printf("Subtract: %d\n", op(5, 3));

    op = multiply;
    printf("Multiply: %d\n", op(5, 3));

    op = divide;
    printf("Divide: %d\n", op(6, 2));

    // Using the processing function
    printf("\nUsing process_operation:\n");
    process_operation(add, 10, 5);
    process_operation(multiply, 10, 5);

    // Logger typedef
    Logger logger = (Logger) log_message;
    logger("This is a log message");
    logger("Another log entry");
    printf("\n");

    // 8. Function pointer array
    printf("--- Function Pointer Array ---\n");

    FunctionArray operations = {add, subtract, multiply, divide};
    char* op_names[] = {"Addition", "Subtraction", "Multiplication", "Division"};

    int x = 10, y = 2;
    for (int i = 0; i < 4; i++)
    {
        printf("%s: %d %s %d = %d\n",
               op_names[i],
               x,
               i == 0 ? "+" : (i == 1 ? "-" : (i == 2 ? "*" : "/")),
               y,
               operations[i](x, y));
    }

    return 0;
}
