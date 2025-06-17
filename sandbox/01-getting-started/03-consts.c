#include <stdio.h>

#define PI 3.14159265358979323846

/**
 * Circle Area Calculator
 * 
 * This program demonstrates:
 * - Using defined constants
 * - User input with validation
 * - Mathematical calculations
 * - Formatted output
 */
int main(int argc, char *argv[]) {
    float radius, area;
    printf("Please enter the radius of the circle: ");
    if (scanf("f", &radius) != 1) {
        printf("Invalid input, please enter a number. \n");
        return 1;
    }

    if (radius < 0) {
        printf("Error: Radius must be a positive number.\n");
        return 1;
    }

    area = PI * radius * radius;
    printf("\nCircle properties:\n");
    printf("- Radius: %.2f units\n", radius);
    printf("- Area: %.2f square units\n", area);
    printf("- Circumference: %.2f units\n", 2 * PI * radius);

    return 0;
}