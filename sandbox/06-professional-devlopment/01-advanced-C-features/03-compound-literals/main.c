#include <stdio.h>
#include <string.h>

// Define some structs for demonstration
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

typedef struct
{
    char name[50];
    int age;
    float score;
} Student;

// Function that accepts a Point
void print_point(Point p)
{
    printf("Point(%d, %d)\n", p.x, p.y);
}

// Function that accepts a Point pointer
void move_point(Point *p, int dx, int dy)
{
    p->x += dx;
    p->y += dy;
}

// Function that accepts an array of integers
void print_array(int arr[], int size)
{
    printf("Array: [");
    for (int i = 0; i < size; i++)
    {
        printf("%d%s", arr[i], i < size - 1 ? ", " : "");
    }
    printf("]\n");
}

// Basic compound literals demo
void basic_compound_literals()
{
    printf("=== Basic Compound Literals ===\n");

    // Using compound literal for a struct
    printf("Point compound literal:\n");
    print_point((Point) {10, 20});

    // Using compound literal for an array
    printf("\nArray compound literal:\n");
    print_array((int[]) {1, 2, 3, 4, 5}, 5);

    // Assigning compound literal to a variable
    printf("\nAssigning to a variable:\n");
    Point p = (Point) {15, 25};
    print_point(p);
}

// Advanced usage with nested structs
void advanced_compound_literals()
{
    printf("\n=== Advanced Compound Literals ===\n");

    // Create a Rectangle using compound literals
    printf("Rectangle with nested Point literals:\n");
    Rectangle rect = (Rectangle) {.top_left = (Point) {10, 10},
                                  .bottom_right = (Point) {20, 20}};

    printf("Rectangle: top_left(%d, %d), bottom_right(%d, %d)\n",
           rect.top_left.x,
           rect.top_left.y,
           rect.bottom_right.x,
           rect.bottom_right.y);
}

// Using compound literals with arrays
void compound_literals_with_arrays()
{
    printf("\n=== Compound Literals with Arrays ===\n");

    // Create an array of Points
    printf("Array of Points:\n");
    Point *points = (Point[]) {{1, 1}, {2, 2}, {3, 3}, {4, 4}};

    // Print the points
    for (int i = 0; i < 4; i++)
    {
        printf("  ");
        print_point(points[i]);
    }

    // 2D array with compound literal
    printf("\n2D array:\n");
    int (*matrix)[3] = (int[][3]) {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    // Print the matrix
    for (int i = 0; i < 3; i++)
    {
        printf("  [");
        for (int j = 0; j < 3; j++)
        {
            printf("%d%s", matrix[i][j], j < 2 ? ", " : "");
        }
        printf("]\n");
    }
}

// Using compound literals in function arguments
void compound_literals_in_functions()
{
    printf("\n=== Compound Literals in Function Arguments ===\n");

    // Pass a compound literal to a function
    printf("As direct argument:\n");
    print_point((Point) {42, 73});

    // Pass a compound literal as a pointer
    printf("\nAs pointer argument:\n");
    Point *p = &(Point) {5, 5};
    printf("Before move: ");
    print_point(*p);

    move_point(p, 3, 3);

    printf("After move:  ");
    print_point(*p);
}

// Using compound literals with strings and character arrays
void compound_literals_with_strings()
{
    printf("\n=== Compound Literals with Strings ===\n");

    // Character array compound literal
    printf("Character array literal:\n");
    char *message = (char[]) {'H', 'e', 'l', 'l', 'o', '\0'};
    printf("  Message: %s\n", message);

    // String compound literal is simpler
    printf("\nString literal:\n");
    char *greeting = (char[]) {"Hello, compound literals!"};
    printf("  Greeting: %s\n", greeting);

    // Using strcpy with compound literal
    char *copied = strcpy((char[100]) {""}, "Copying to compound literal");
    printf("\nAfter strcpy(): %s\n", copied);
}

// Lifetime and scope demonstration
void lifetime_of_compound_literals()
{
    printf("\n=== Modifying Compound Literals ===\n");

    // Demonstrate modifying a compound literal
    int *arr = (int[]) {10, 20, 30, 40};

    printf("Original: [%d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3]);

    // Modify the array
    arr[1] = 200;
    arr[3] = 400;

    printf("Modified: [%d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3]);
}

int main()
{
    printf("==== COMPOUND LITERALS DEMO ====\n\n");

    basic_compound_literals();
    advanced_compound_literals();
    compound_literals_with_arrays();
    compound_literals_in_functions();
    compound_literals_with_strings();
    lifetime_of_compound_literals();

    return 0;
}
