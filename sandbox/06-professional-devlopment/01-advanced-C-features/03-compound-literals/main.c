#include <stdio.h>
#include <string.h>

// Define some structs for demonstration
typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point top_left;
    Point bottom_right;
} Rectangle;

typedef struct {
    char name[50];
    int age;
    float score;
} Student;

// Function that accepts a Point
void print_point(Point p) {
    printf("Point(%d, %d)\n", p.x, p.y);
}

// Function that accepts a Point pointer
void move_point(Point *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

// Function that accepts an array of integers
void print_array(int arr[], int size) {
    printf("Array: [");
    for (int i = 0; i < size; i++) {
        printf("%d%s", arr[i], i < size - 1 ? ", " : "");
    }
    printf("]\n");
}

// Basic compound literals demo
void basic_compound_literals() {
    printf("=== Basic Compound Literals ===\n");
    
    // Using compound literal for a struct
    printf("Creating and using a Point compound literal:\n");
    print_point((Point){10, 20});
    
    // Using compound literal for an array
    printf("\nCreating and using an array compound literal:\n");
    print_array((int[]){1, 2, 3, 4, 5}, 5);
    
    // Assigning compound literal to a variable
    printf("\nAssigning compound literal to a variable:\n");
    Point p = (Point){15, 25};
    print_point(p);
}

// Advanced usage with nested structs
void advanced_compound_literals() {
    printf("\n=== Advanced Compound Literals ===\n");
    
    // Create a Rectangle using compound literals
    printf("Creating a Rectangle with nested Point compound literals:\n");
    Rectangle rect = (Rectangle){
        .top_left = (Point){10, 10},
        .bottom_right = (Point){20, 20}
    };
    
    printf("Rectangle: top_left(%d, %d), bottom_right(%d, %d)\n",
           rect.top_left.x, rect.top_left.y,
           rect.bottom_right.x, rect.bottom_right.y);
}

// Using compound literals with arrays
void compound_literals_with_arrays() {
    printf("\n=== Compound Literals with Arrays ===\n");
    
    // Create an array of Points
    printf("Creating an array of Points:\n");
    Point *points = (Point[]){
        {1, 1},
        {2, 2},
        {3, 3},
        {4, 4}
    };
    
    // Print the points
    for (int i = 0; i < 4; i++) {
        printf("  ");
        print_point(points[i]);
    }
    
    // 2D array with compound literal
    printf("\nCreating a 2D array:\n");
    int (*matrix)[3] = (int[][3]){
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    
    // Print the matrix
    for (int i = 0; i < 3; i++) {
        printf("  [");
        for (int j = 0; j < 3; j++) {
            printf("%d%s", matrix[i][j], j < 2 ? ", " : "");
        }
        printf("]\n");
    }
}

// Using compound literals in function arguments
void compound_literals_in_functions() {
    printf("\n=== Compound Literals in Function Arguments ===\n");
    
    // Pass a compound literal to a function
    printf("Passing a Point compound literal to print_point():\n");
    print_point((Point){42, 73});
    
    // Pass a compound literal as a pointer
    printf("\nPassing a Point compound literal to move_point():\n");
    Point *p = &(Point){5, 5};
    printf("Before move: ");
    print_point(*p);
    
    move_point(p, 3, 3);
    
    printf("After move:  ");
    print_point(*p);
}

// Using compound literals with strings and character arrays
void compound_literals_with_strings() {
    printf("\n=== Compound Literals with Strings ===\n");
    
    // Character array compound literal
    printf("Creating a char array compound literal:\n");
    char *message = (char[]){'H', 'e', 'l', 'l', 'o', '\0'};
    printf("  Message: %s\n", message);
    
    // String compound literal is simpler
    printf("\nCreating a string compound literal:\n");
    char *greeting = (char[]){"Hello, compound literals!"};
    printf("  Greeting: %s\n", greeting);
    
    // Using strcpy with compound literal
    printf("\nUsing strcpy() with compound literal as destination:\n");
    strcpy((char[100]){""}, "This works but is unusual!");
}

// Lifetime and scope of compound literals
void lifetime_of_compound_literals() {
    printf("\n=== Lifetime and Scope of Compound Literals ===\n");
    
    printf("1. Block scope compound literals exist until end of block\n");
    printf("2. File scope compound literals exist for the program lifetime\n");
    printf("3. Taking the address of a compound literal is valid\n");
    printf("4. Modifying a compound literal is allowed\n");
    
    // Demonstrate modifying a compound literal
    printf("\nModifying a compound literal:\n");
    int *arr = (int[]){10, 20, 30, 40};
    
    printf("  Original: [%d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3]);
    
    // Modify the array
    arr[1] = 200;
    arr[3] = 400;
    
    printf("  Modified: [%d, %d, %d, %d]\n", arr[0], arr[1], arr[2], arr[3]);
}

// Best practices and common patterns
void compound_literal_best_practices() {
    printf("\n=== Compound Literals: Best Practices ===\n");
    
    printf("Common uses for compound literals:\n");
    printf("1. Temporary structs for function arguments\n");
    printf("2. Creating test data or example values\n");
    printf("3. One-off arrays and buffers\n");
    printf("4. Default/initial values\n");
    
    printf("\nBest practices:\n");
    printf("1. Use for short-lived, temporary objects\n");
    printf("2. Be cautious with pointers to compound literals\n");
    printf("3. Consider readability - complex literals can be hard to read\n");
    printf("4. Combine with designated initializers for clarity\n");
    
    printf("\nAvoid these patterns:\n");
    printf("1. Returning pointers to block-scope compound literals\n");
    printf("2. Creating extremely large compound literals\n");
    printf("3. Deeply nested compound literals without formatting\n");
}

int main() {
    printf("==== COMPOUND LITERALS ====\n\n");
    
    basic_compound_literals();
    advanced_compound_literals();
    compound_literals_with_arrays();
    compound_literals_in_functions();
    compound_literals_with_strings();
    lifetime_of_compound_literals();
    compound_literal_best_practices();
    
    return 0;
}